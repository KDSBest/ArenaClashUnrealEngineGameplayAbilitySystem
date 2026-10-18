#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerState.h"
#include "AbilitySystemInterface.h"
#include "ArenaPlayerState.generated.h"

class UAbilitySystemComponent;

UCLASS()
class ARENACLASH_API AArenaPlayerState : public APlayerState,
										 public IAbilitySystemInterface
{
	GENERATED_BODY()

public:
	AArenaPlayerState();

	// IAbilitySystemInterface
	virtual UAbilitySystemComponent* GetAbilitySystemComponent() const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;
};
