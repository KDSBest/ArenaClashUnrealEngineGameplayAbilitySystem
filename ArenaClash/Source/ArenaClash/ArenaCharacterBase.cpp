#include "ArenaCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "ArenaAttributeSet.h"

AArenaCharacterBase::AArenaCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Turn the character toward the direction it moves,
	// not toward where the camera looks.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.f, 500.f, 0.f);
	Movement->JumpZVelocity = 700.f;
	Movement->MaxWalkSpeed = 500.f;
}

UAbilitySystemComponent* AArenaCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AArenaCharacterBase::ApplyDefaultAttributes()
{
	if (!HasAuthority() || !AbilitySystemComponent || !DefaultAttributesEffect)
	{
		return;
	}

	FGameplayEffectContextHandle Context =
		AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	const FGameplayEffectSpecHandle Spec =
		AbilitySystemComponent->MakeOutgoingSpec(DefaultAttributesEffect, 1.f, Context);

	if (Spec.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}
}

void AArenaCharacterBase::BindAttributeCallbacks()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	FOnGameplayAttributeValueChange& Delegate =
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UArenaAttributeSet::GetMoveSpeedAttribute());

	// Avoid binding twice if InitAbilityActorInfo runs again.
	Delegate.RemoveAll(this);
	Delegate.AddUObject(this, &AArenaCharacterBase::OnMoveSpeedChanged);
}

void AArenaCharacterBase::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}
