#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "ArenaCharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;
class UArenaGameplayAbility;

UCLASS(Abstract)
class ARENACLASH_API AArenaCharacterBase : public ACharacter,
										  public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AArenaCharacterBase();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

	// Server only. Called by the attribute set when Health reaches 0.
	virtual void Die(AActor* Killer);

	UFUNCTION(BlueprintPure, Category = "Arena")
	bool IsDead() const;

	UFUNCTION(BlueprintPure, Category = "Team")
	virtual uint8 GetTeamId() const { return 0; }

	// Server only. Remembers who damaged this character, and when.
	void RecordDamageFrom(AActor* DamageInstigator);

protected:
	// Connects this Character to its ASC. Each child class
	// decides where the ASC comes from.
	virtual void InitAbilityActorInfo() {}

	// Applied once on the server to set starting attribute values.
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	// Applied once on the server after the default attributes.
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UGameplayEffect>> StartupEffects;

	void ApplyDefaultAttributes();

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TArray<TSubclassOf<UArenaGameplayAbility>> StartupAbilities;

	void GrantStartupAbilities();

	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DeathEffect;

	TMap<TWeakObjectPtr<AActor>, double> RecentDamagers;

	// Call after InitAbilityActorInfo on every machine.
	void BindAttributeCallbacks();

	void OnMoveSpeedChanged(const FOnAttributeChangeData& Data);

	virtual void OnStunTagChanged(const FGameplayTag Tag, int32 NewCount);
	virtual void OnShieldTagChanged(const FGameplayTag Tag, int32 NewCount);

	// A pointer to the ASC, wherever it lives.
	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
