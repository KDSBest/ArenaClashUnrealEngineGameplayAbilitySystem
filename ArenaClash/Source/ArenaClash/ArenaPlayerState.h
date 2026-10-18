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

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Team")
	uint8 TeamId = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	int32 Kills = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	int32 Deaths = 0;

	UPROPERTY(Replicated, BlueprintReadOnly, Category = "Score")
	int32 Assists = 0;

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> AbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UArenaAttributeSet> AttributeSet;
};
