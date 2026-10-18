#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameStateBase.h"
#include "ArenaGameState.generated.h"

UENUM(BlueprintType)
enum class EArenaMatchPhase : uint8
{
	Warmup,
	InProgress,
	Ended
};

UCLASS()
class ARENACLASH_API AArenaGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	UPROPERTY(ReplicatedUsing = OnRep_MatchPhase, BlueprintReadOnly)
	EArenaMatchPhase MatchPhase = EArenaMatchPhase::Warmup;

	// Server time at which the current phase ends.
	UPROPERTY(Replicated, BlueprintReadOnly)
	float PhaseEndTime = 0.f;

	UPROPERTY(Replicated, BlueprintReadOnly)
	TArray<int32> TeamScores; // index = TeamId

	UPROPERTY(Replicated, BlueprintReadOnly)
	uint8 WinningTeam = 0;

	void AddTeamScore(uint8 TeamId, int32 Amount);

	UFUNCTION(BlueprintPure)
	float GetPhaseTimeRemaining() const;

	// Safe read of TeamScores; 0 for teams without a score yet.
	UFUNCTION(BlueprintPure)
	int32 GetTeamScore(uint8 TeamId) const
	{
		return TeamScores.IsValidIndex(TeamId) ? TeamScores[TeamId] : 0;
	}

protected:
	UFUNCTION()
	void OnRep_MatchPhase();
};
