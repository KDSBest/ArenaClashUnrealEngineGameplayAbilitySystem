#include "ArenaGameState.h"
#include "Net/UnrealNetwork.h"

void AArenaGameState::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArenaGameState, MatchPhase);
	DOREPLIFETIME(AArenaGameState, PhaseEndTime);
	DOREPLIFETIME(AArenaGameState, TeamScores);
	DOREPLIFETIME(AArenaGameState, WinningTeam);
}

void AArenaGameState::AddTeamScore(uint8 TeamId, int32 Amount)
{
	if (TeamScores.Num() <= TeamId)
	{
		TeamScores.SetNumZeroed(TeamId + 1);
	}
	TeamScores[TeamId] += Amount;
}

float AArenaGameState::GetPhaseTimeRemaining() const
{
	return FMath::Max(0.f, PhaseEndTime - GetServerWorldTimeSeconds());
}

void AArenaGameState::OnRep_MatchPhase()
{
	// The HUD reads MatchPhase directly; Chapter 21 reacts to changes here.
}
