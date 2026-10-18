#include "ArenaPlayerState.h"
#include "AbilitySystemComponent.h"

AArenaPlayerState::AArenaPlayerState()
{
	AbilitySystemComponent =
		CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	// PlayerStates update slowly by default. GAS needs faster updates.
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AArenaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
