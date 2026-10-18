#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameState.h"
#include "ArenaGameMode.generated.h"

class AArenaCharacterBase;
class UGameplayEffect;

UCLASS()
class ARENACLASH_API AArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArenaGameMode();

	void RequestRespawn(AController* Controller);

	bool IsFriendlyFireEnabled() const { return bFriendlyFire; }

	// Server only. Called by a character when it dies.
	void OnCharacterKilled(AArenaCharacterBase* Victim, AActor* Killer,
						   const TArray<AActor*>& Assisters);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	float RespawnDelay = 5.f;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	// PvP: pick a Player Start tagged Team1 / Team2 for the player's team.
	virtual AActor* ChoosePlayerStart_Implementation(AController* Player) override;

	// true: everyone on team 1 against AI; false: two teams.
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	bool bCoop = true;

	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	bool bFriendlyFire = false;

	virtual void BeginPlay() override;

	void SetMatchPhase(EArenaMatchPhase NewPhase, float Duration);
	void OnPhaseTimerExpired();
	void CheckMatchEnd();
	void EndMatch();

	UPROPERTY(EditDefaultsOnly, Category = "Arena|Match")
	float WarmupDuration = 15.f;

	UPROPERTY(EditDefaultsOnly, Category = "Arena|Match")
	float MatchDuration = 300.f;

	UPROPERTY(EditDefaultsOnly, Category = "Arena|Match")
	float EndDuration = 10.f;

	UPROPERTY(EditDefaultsOnly, Category = "Arena|Match")
	int32 ScoreLimit = 20;

	UPROPERTY(EditDefaultsOnly, Category = "Arena|Match")
	TSubclassOf<UGameplayEffect> MatchOverEffect;

	FTimerHandle PhaseTimer;
};
