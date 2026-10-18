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

	virtual const FGameplayTagContainer* GetCooldownTags() const override;

	virtual void ApplyCooldown(
		const FGameplayAbilitySpecHandle Handle,
		const FGameplayAbilityActorInfo* ActorInfo,
		const FGameplayAbilityActivationInfo ActivationInfo) const override;

protected:
	// Tag granted while on cooldown, e.g. Cooldown.Mage.Fireball.
	UPROPERTY(EditDefaultsOnly, Category = "Cooldowns",
			  meta = (Categories = "Cooldown"))
	FGameplayTagContainer CooldownTags;

	UPROPERTY(EditDefaultsOnly, Category = "Cooldowns")
	FScalableFloat CooldownDuration;

private:
	// Scratch container returned by GetCooldownTags.
	UPROPERTY(Transient)
	FGameplayTagContainer TempCooldownTags;
};
