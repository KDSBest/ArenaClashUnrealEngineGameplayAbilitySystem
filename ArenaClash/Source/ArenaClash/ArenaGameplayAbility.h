#pragma once

#include "CoreMinimal.h"
#include "Abilities/GameplayAbility.h"
#include "ArenaGameplayAbility.generated.h"

UCLASS(Abstract)
class ARENACLASH_API UArenaGameplayAbility : public UGameplayAbility
{
	GENERATED_BODY()

public:
	UArenaGameplayAbility();

	// The input that activates this ability, e.g. Input.Ability.Primary.
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "Input")
	FGameplayTag InputTag;
};
