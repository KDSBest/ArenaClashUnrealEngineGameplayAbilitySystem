#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ArenaPlayerState.generated.h"

class UAbilitySystemComponent;
class UArenaAttributeSet;

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

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UArenaAttributeSet> AttributeSet;
};
