#include "ArenaPlayerCharacter.h"
#include "Camera/CameraComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "InputActionValue.h"
#include "AbilitySystemComponent.h"
#include "ArenaPlayerState.h"

AArenaPlayerCharacter::AArenaPlayerCharacter()
{
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.f;
	CameraBoom->bUsePawnControlRotation = true;

	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;
}

void AArenaPlayerCharacter::PossessedBy(AController* NewController)
{
	Super::PossessedBy(NewController);

	// Server.
	InitAbilityActorInfo();
	ApplyDefaultAttributes();
	GrantStartupAbilities();
}

void AArenaPlayerCharacter::OnRep_PlayerState()
{
	Super::OnRep_PlayerState();

	// Owning client (and every other client that receives the PlayerState).
	InitAbilityActorInfo();
}

void AArenaPlayerCharacter::InitAbilityActorInfo()
{
	AArenaPlayerState* PS = GetPlayerState<AArenaPlayerState>();
	if (!PS)
	{
		return;
	}

	AbilitySystemComponent = PS->GetAbilitySystemComponent();
	AbilitySystemComponent->InitAbilityActorInfo(PS, this);
	BindAttributeCallbacks();
}

void AArenaPlayerCharacter::SetupPlayerInputComponent(
	UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	UEnhancedInputComponent* Input =
		Cast<UEnhancedInputComponent>(PlayerInputComponent);
	if (!Input)
	{
		return;
	}

	Input->BindAction(MoveAction, ETriggerEvent::Triggered,
					  this, &AArenaPlayerCharacter::Move);
	Input->BindAction(LookAction, ETriggerEvent::Triggered,
					  this, &AArenaPlayerCharacter::Look);
	Input->BindAction(JumpAction, ETriggerEvent::Started,
					  this, &ACharacter::Jump);
	Input->BindAction(JumpAction, ETriggerEvent::Completed,
					  this, &ACharacter::StopJumping);

	for (const FArenaAbilityInputBinding& Binding : AbilityInputBindings)
	{
		if (!Binding.InputAction || !Binding.InputTag.IsValid())
		{
			continue;
		}

		Input->BindAction(Binding.InputAction, ETriggerEvent::Started, this,
			&AArenaPlayerCharacter::AbilityInputPressed, Binding.InputTag);
		Input->BindAction(Binding.InputAction, ETriggerEvent::Completed, this,
			&AArenaPlayerCharacter::AbilityInputReleased, Binding.InputTag);
	}
}

void AArenaPlayerCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	if (!Controller)
	{
		return;
	}

	// Move relative to where the camera is facing, ignoring pitch.
	const FRotator YawRotation(0.f, Controller->GetControlRotation().Yaw, 0.f);
	const FVector Forward = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector Right = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(Forward, Input.Y);
	AddMovementInput(Right, Input.X);
}

void AArenaPlayerCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D Input = Value.Get<FVector2D>();
	AddControllerYawInput(Input.X);
	AddControllerPitchInput(Input.Y);
}

void AArenaPlayerCharacter::AbilityInputPressed(FGameplayTag InputTag)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	for (FGameplayAbilitySpec& Spec :
		 AbilitySystemComponent->GetActivatableAbilities())
	{
		if (!Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			continue;
		}

		AbilitySystemComponent->AbilitySpecInputPressed(Spec);

		if (!Spec.IsActive())
		{
			AbilitySystemComponent->TryActivateAbility(Spec.Handle);
		}
	}
}

void AArenaPlayerCharacter::AbilityInputReleased(FGameplayTag InputTag)
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	for (FGameplayAbilitySpec& Spec :
		 AbilitySystemComponent->GetActivatableAbilities())
	{
		if (Spec.GetDynamicSpecSourceTags().HasTagExact(InputTag))
		{
			AbilitySystemComponent->AbilitySpecInputReleased(Spec);
		}
	}
}
