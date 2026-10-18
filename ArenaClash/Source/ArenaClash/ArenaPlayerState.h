#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ArenaAbilitySet.h"
#include "ArenaPlayerState.generated.h"

class UAbilitySystemComponent;
class UArenaAttributeSet;
class UArenaHeroData;

UCLASS()
class ARENACLASH_API AArenaPlayerState : public APlayerState,
										 public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AArenaPlayerState();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	UArenaAttributeSet* GetAttributeSet() const { return AttributeSet; }

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Team")
	uint8 TeamId = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	int32 Kills = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	int32 Deaths = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	int32 Assists = 0;

	UPROPERTY(ReplicatedUsing = OnRep_HeroData, BlueprintReadOnly, Category = "Hero")
	TObjectPtr<UArenaHeroData> HeroData;

	// Server only: switch to a new hero, replacing all hero abilities.
	void SetHeroData(UArenaHeroData* NewHero);

	virtual void CopyProperties(APlayerState* NewPlayerState) override;
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UArenaAttributeSet> AttributeSet;

	UFUNCTION()
	void OnRep_HeroData();

	FArenaAbilitySetHandles GrantedHeroHandles;
};
