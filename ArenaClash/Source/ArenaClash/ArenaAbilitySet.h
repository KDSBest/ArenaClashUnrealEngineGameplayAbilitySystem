#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GameplayAbilitySpecHandle.h"
#include "ActiveGameplayEffectHandle.h"
#include "GameplayTagContainer.h"
#include "ArenaAbilitySet.generated.h"

class UAbilitySystemComponent;
class UArenaGameplayAbility;
class UGameplayEffect;

USTRUCT(BlueprintType)
struct FArenaAbilitySet_Ability
{
	GENERATED_BODY()

	UPROPERTY(EditDefaultsOnly)
	TSubclassOf<UArenaGameplayAbility> Ability;

	UPROPERTY(EditDefaultsOnly)
	int32 Level = 1;

	// Leave empty to use the ability's own InputTag.
	UPROPERTY(EditDefaultsOnly, meta = (Categories = "Input.Ability"))
	FGameplayTag InputTagOverride;
};

// Everything a set granted, so it can be taken away again.
USTRUCT()
struct FArenaAbilitySetHandles
{
	GENERATED_BODY()

	TArray<FGameplayAbilitySpecHandle> Abilities;
	TArray<FActiveGameplayEffectHandle> Effects;

	void TakeFromAbilitySystem(UAbilitySystemComponent* ASC);
};

UCLASS(BlueprintType)
class ARENACLASH_API UArenaAbilitySet : public UPrimaryDataAsset
{
	GENERATED_BODY()

public:
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<FArenaAbilitySet_Ability> Abilities;

	// Usually infinite effects, such as passive regeneration.
	UPROPERTY(EditDefaultsOnly, Category = "Effects")
	TArray<TSubclassOf<UGameplayEffect>> Effects;

	// Server only.
	void GiveToAbilitySystem(UAbilitySystemComponent* ASC,
							 FArenaAbilitySetHandles* OutHandles) const;
};
