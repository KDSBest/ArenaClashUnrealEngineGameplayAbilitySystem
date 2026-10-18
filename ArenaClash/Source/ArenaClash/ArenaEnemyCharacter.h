#pragma once

#include "CoreMinimal.h"
#include "ArenaCharacterBase.h"
#include "ArenaEnemyCharacter.generated.h"

UCLASS()
class ARENACLASH_API AArenaEnemyCharacter : public AArenaCharacterBase
{
	GENERATED_BODY()

public:
	AArenaEnemyCharacter();

protected:
	virtual void BeginPlay() override;
	virtual void InitAbilityActorInfo() override;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Abilities")
	TObjectPtr<UAbilitySystemComponent> EnemyAbilitySystemComponent;
};
