#pragma once

#include "CoreMinimal.h"
#include "ArenaCharacterBase.h"
#include "ArenaEnemyCharacter.generated.h"

class UArenaAttributeSet;

UCLASS()
class ARENACLASH_API AArenaEnemyCharacter : public AArenaCharacterBase
{
	GENERATED_BODY()

public:
	AArenaEnemyCharacter();

	virtual void Die(AActor* Killer) override;

	virtual uint8 GetTeamId() const override { return 255; }

	// Scales the default attributes through curve tables. Set by the wave spawner.
	UPROPERTY(BlueprintReadWrite, Category = "Enemy", meta = (ExposeOnSpawn = true))
	int32 EnemyLevel = 1;

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> EnemyAbilitySystemComponent;

	UPROPERTY()
	TObjectPtr<UArenaAttributeSet> EnemyAttributeSet;
};
