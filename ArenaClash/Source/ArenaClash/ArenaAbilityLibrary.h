#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "GameplayTagContainer.h"
#include "ArenaAbilityLibrary.generated.h"

class UAbilitySystemComponent;

UCLASS()
class ARENACLASH_API UArenaAbilityLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()

public:
	// Returns true if a cooldown with this tag is active.
	UFUNCTION(BlueprintCallable, Category = "Arena|Abilities")
	static bool GetCooldownRemaining(UAbilitySystemComponent* ASC,
									 FGameplayTag CooldownTag,
									 float& TimeRemaining,
									 float& Duration);
};
