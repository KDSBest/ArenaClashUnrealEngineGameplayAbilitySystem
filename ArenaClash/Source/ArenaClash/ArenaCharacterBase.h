#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "GameplayEffectTypes.h"
#include "ArenaCharacterBase.generated.h"

class UAbilitySystemComponent;
class UGameplayEffect;

UCLASS(Abstract)
class ARENACLASH_API AArenaCharacterBase : public ACharacter,
										  public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AArenaCharacterBase();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	// Connects this Character to its ASC. Each child class
	// decides where the ASC comes from.
	virtual void InitAbilityActorInfo() {}

	// Applied once on the server to set starting attribute values.
	UPROPERTY(EditDefaultsOnly, Category = "Abilities")
	TSubclassOf<UGameplayEffect> DefaultAttributesEffect;

	void ApplyDefaultAttributes();

	// Call after InitAbilityActorInfo on every machine.
	void BindAttributeCallbacks();

	void OnMoveSpeedChanged(const FOnAttributeChangeData& Data);

	// A pointer to the ASC, wherever it lives.
	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
