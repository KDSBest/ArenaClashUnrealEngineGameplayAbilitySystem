#include "ArenaPlayerState.h"
#include "AbilitySystemComponent.h"
#include "ArenaAttributeSet.h"
#include "Net/UnrealNetwork.h"
#include "ArenaHeroData.h"
#include "ArenaPlayerController.h"

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
	// The new pawn can reach the owning client before the new hero does,
	// so the HUD was built for the old hero. Rebuild it now.
	AArenaPlayerController* PC = Cast<AArenaPlayerController>(GetOwningController());
	if (PC && PC->IsLocalController())
	{
		PC->OnHeroReady(AbilitySystemComponent);
	}
}

void AArenaPlayerState::CopyProperties(APlayerState* NewPlayerState)
{
	Super::CopyProperties(NewPlayerState);

	if (AArenaPlayerState* NewPS = Cast<AArenaPlayerState>(NewPlayerState))
	{
		NewPS->SetHeroData(HeroData);
		NewPS->TeamId = TeamId;
	}
}
