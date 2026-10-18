#include "ArenaPlayerState.h"
#include "AbilitySystemComponent.h"
#include "ArenaAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "ArenaHeroData.h"

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

void AArenaPlayerState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArenaPlayerState, TeamId);
	DOREPLIFETIME(AArenaPlayerState, Kills);
	DOREPLIFETIME(AArenaPlayerState, Deaths);
	DOREPLIFETIME(AArenaPlayerState, Assists);
	DOREPLIFETIME(AArenaPlayerState, HeroData);
}

void AArenaPlayerState::SetHeroData(UArenaHeroData* NewHero)
{
	if (!HasAuthority() || NewHero == HeroData)
	{
		return;
	}

	GrantedHeroHandles.TakeFromAbilitySystem(AbilitySystemComponent);

	HeroData = NewHero;

	if (HeroData)
	{
		for (const UArenaAbilitySet* Set : HeroData->AbilitySets)
		{
			if (Set)
			{
				Set->GiveToAbilitySystem(AbilitySystemComponent, &GrantedHeroHandles);
			}
		}
	}
	OnRep_HeroData();
}

void AArenaPlayerState::OnRep_HeroData()
{
	// Chapter 21: notify the UI that the hero changed.
}
