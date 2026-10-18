#include "ArenaAbilityTask_WaitLanded.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"

UArenaAbilityTask_WaitLanded* UArenaAbilityTask_WaitLanded::WaitLanded(
	UGameplayAbility* OwningAbility)
{
	return NewAbilityTask<UArenaAbilityTask_WaitLanded>(OwningAbility);
}

void UArenaAbilityTask_WaitLanded::Activate()
{
	ACharacter* Character = Cast<ACharacter>(GetAvatarActor());
	if (!Character)
	{
		EndTask();
		return;
	}

	if (Character->GetCharacterMovement()->IsMovingOnGround())
	{
		Finish();
		return;
	}

	Character->LandedDelegate.AddDynamic(
		this, &UArenaAbilityTask_WaitLanded::HandleLanded);
}

void UArenaAbilityTask_WaitLanded::HandleLanded(const FHitResult& Hit)
{
	Finish();
}

void UArenaAbilityTask_WaitLanded::Finish()
{
	if (ShouldBroadcastAbilityTaskDelegates())
	{
		OnLanded.Broadcast();
	}
	EndTask();
}

void UArenaAbilityTask_WaitLanded::OnDestroy(bool bInOwnerFinished)
{
	if (ACharacter* Character = Cast<ACharacter>(GetAvatarActor()))
	{
		Character->LandedDelegate.RemoveDynamic(
			this, &UArenaAbilityTask_WaitLanded::HandleLanded);
	}
	Super::OnDestroy(bInOwnerFinished);
}
