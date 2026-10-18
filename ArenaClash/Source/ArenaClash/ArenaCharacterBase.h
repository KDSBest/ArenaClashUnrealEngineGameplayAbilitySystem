#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "AbilitySystemInterface.h"
#include "ArenaCharacterBase.generated.h"

class UAbilitySystemComponent;

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

	// A pointer to the ASC, wherever it lives.
	UPROPERTY(Transient)
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
