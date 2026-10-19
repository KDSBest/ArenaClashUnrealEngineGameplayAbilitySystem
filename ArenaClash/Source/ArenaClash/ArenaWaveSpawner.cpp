#include "ArenaWaveSpawner.h"
#include "ArenaEnemyCharacter.h"
#include "ArenaGameMode.h"
#include "ArenaGameState.h"
#include "TimerManager.h"

void AArenaWaveSpawner::StartWaves()
{
	CurrentWave = 0;
	StartNextWave();
}

void AArenaWaveSpawner::StartNextWave()
{
	if (!Waves.IsValidIndex(CurrentWave))
	{
		if (AArenaGameMode* GM = GetWorld()->GetAuthGameMode<AArenaGameMode>())
		{
			GM->OnAllWavesCleared();
		}
		return;
	}

	GetWorldTimerManager().SetTimer(WaveTimer, this,
		&AArenaWaveSpawner::SpawnWave, Waves[CurrentWave].DelayBefore, false);
}

void AArenaWaveSpawner::SpawnWave()
{
	const FArenaWave& Wave = Waves[CurrentWave];
	++CurrentWave;

	if (AArenaGameState* GS = GetWorld()->GetGameState<AArenaGameState>())
	{
		GS->CurrentWave = CurrentWave;
	}

	int32 PointIndex = 0;
	for (const FArenaWaveEntry& Entry : Wave.Enemies)
	{
		for (int32 i = 0; i < Entry.Count && SpawnPoints.Num() > 0; ++i)
		{
			const FTransform Transform =
				SpawnPoints[PointIndex++ % SpawnPoints.Num()]->GetActorTransform();

			AArenaEnemyCharacter* Enemy =
				GetWorld()->SpawnActorDeferred<AArenaEnemyCharacter>(
					Entry.EnemyClass, Transform, nullptr, nullptr,
					ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn);
			if (!Enemy)
			{
				continue;
			}

			Enemy->EnemyLevel = CurrentWave;
			Enemy->OnDied.AddUObject(this, &AArenaWaveSpawner::HandleEnemyDied);
			Enemy->FinishSpawning(Transform);
			++AliveEnemies;
		}
	}
}

void AArenaWaveSpawner::HandleEnemyDied(AArenaCharacterBase* Enemy)
{
	if (--AliveEnemies <= 0)
	{
		if (AArenaGameMode* GM = GetWorld()->GetAuthGameMode<AArenaGameMode>())
		{
			GM->OnWaveCleared(CurrentWave);
		}
		StartNextWave();
	}
}
