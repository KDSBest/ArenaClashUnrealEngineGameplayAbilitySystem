#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaWaveSpawner.generated.h"

class AArenaCharacterBase;
class AArenaEnemyCharacter;

USTRUCT(BlueprintType)
struct FArenaWaveEntry
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TSubclassOf<AArenaEnemyCharacter> EnemyClass;

	UPROPERTY(EditAnywhere)
	int32 Count = 3;
};

USTRUCT(BlueprintType)
struct FArenaWave
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TArray<FArenaWaveEntry> Enemies;

	UPROPERTY(EditAnywhere)
	float DelayBefore = 5.f;
};

UCLASS()
class ARENACLASH_API AArenaWaveSpawner : public AActor
{
	GENERATED_BODY()

public:
	// Server only. Called by the GameMode when the match starts.
	void StartWaves();

	int32 GetCurrentWave() const { return CurrentWave; }

protected:
	void StartNextWave();
	void SpawnWave();
	void HandleEnemyDied(AArenaCharacterBase* Enemy);

	UPROPERTY(EditAnywhere, Category = "Waves")
	TArray<FArenaWave> Waves;

	UPROPERTY(EditAnywhere, Category = "Waves")
	TArray<TObjectPtr<AActor>> SpawnPoints;

	int32 CurrentWave = 0;   // 1-based once started
	int32 AliveEnemies = 0;
	FTimerHandle WaveTimer;
};
