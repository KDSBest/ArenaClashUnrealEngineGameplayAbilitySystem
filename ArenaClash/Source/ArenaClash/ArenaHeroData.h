#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ArenaHeroData.generated.h"

class AArenaPlayerCharacter;
class UArenaAbilitySet;
class UGameplayEffect;
class UTexture2D;

UCLASS(BlueprintType)
class ARENACLASH_API UArenaHeroData : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	FText DisplayName;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display",
			  meta = (MultiLine = true))
	FText Description;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Display")
	TObjectPtr<UTexture2D> Portrait;

	// The pawn to spawn for this hero.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<AArenaPlayerCharacter> PawnClass;

	// Applied on every spawn: starting attribute values.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	// Granted once when the hero is chosen.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Gameplay")
	TArray<TObjectPtr<UArenaAbilitySet>> AbilitySets;
};
