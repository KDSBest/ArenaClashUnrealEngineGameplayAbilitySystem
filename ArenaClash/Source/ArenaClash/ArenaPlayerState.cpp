#include "ArenaPlayerState.h"
#include "AbilitySystemComponent.h"
#include "ArenaAttributeSet.h"

AArenaPlayerState::AArenaPlayerState()
{
	AbilitySystemComponent =
		CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	AbilitySystemComponent->SetIsReplicated(true);
	AbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Mixed);

	AttributeSet = CreateDefaultSubobject<UArenaAttributeSet>(TEXT("AttributeSet"));

	// PlayerStates update slowly by default. GAS needs faster updates.
	SetNetUpdateFrequency(100.f);
}

UAbilitySystemComponent* AArenaPlayerState::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}
