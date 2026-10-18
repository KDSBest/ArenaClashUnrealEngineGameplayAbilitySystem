#include "ArenaGameMode.h"
#include "ArenaGameState.h"
#include "ArenaPlayerController.h"
#include "ArenaPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"
#include "ArenaCharacterBase.h"
#include "ArenaAbilityLibrary.h"
#include "ArenaHeroData.h"
#include "ArenaPlayerCharacter.h"
#include "AbilitySystemComponent.h"

AArenaGameMode::AArenaGameMode()
{
	GameStateClass = AArenaGameState::StaticClass();
	PlayerControllerClass = AArenaPlayerController::StaticClass();
	PlayerStateClass = AArenaPlayerState::StaticClass();
}

void AArenaGameMode::RequestRespawn(AController* Controller)
{
	if (!Controller)
	{
		return;
	}

	FTimerHandle Timer;
	FTimerDelegate Delegate = FTimerDelegate::CreateWeakLambda(this,
		[this, Controller]()
		{
			if (APawn* OldPawn = Controller->GetPawn())
			{
				Controller->UnPossess();
				OldPawn->SetLifeSpan(2.f);
			}
			RestartPlayer(Controller);
		});

	GetWorldTimerManager().SetTimer(Timer, Delegate, RespawnDelay, false);
}

void AArenaGameMode::PostLogin(APlayerController* NewPlayer)
{
	// Assign hero and team before Super::PostLogin, because Super spawns
	// the player's first pawn (HandleStartingNewPlayer).
	if (AArenaPlayerState* PS = NewPlayer->GetPlayerState<AArenaPlayerState>())
	{
		if (!PS->HeroData)
		{
			PS->SetHeroData(DefaultHero);
		}

		if (bCoop)
		{
			PS->TeamId = 1;
		}
		else
		{
			// PvP: put the player on the smaller team.
			int32 TeamCount[3] = {0, 0, 0};
			for (APlayerState* Other : GameState->PlayerArray)
			{
				if (const AArenaPlayerState* APS = Cast<AArenaPlayerState>(Other))
				{
					if (APS != PS && APS->TeamId <= 2)
					{
						TeamCount[APS->TeamId]++;
					}
				}
			}
			PS->TeamId = (TeamCount[1] <= TeamCount[2]) ? 1 : 2;
		}
	}

	Super::PostLogin(NewPlayer);
}

AActor* AArenaGameMode::ChoosePlayerStart_Implementation(AController* Player)
{
	const AArenaPlayerState* PS = Player ? Player->GetPlayerState<AArenaPlayerState>() : nullptr;
	if (!bCoop && PS && PS->TeamId > 0)
	{
		const FName TeamTag = *FString::Printf(TEXT("Team%d"), PS->TeamId);

		TArray<APlayerStart*> TeamStarts;
		for (TActorIterator<APlayerStart> It(GetWorld()); It; ++It)
		{
			if (It->PlayerStartTag == TeamTag)
			{
				TeamStarts.Add(*It);
			}
		}
		if (TeamStarts.Num() > 0)
		{
			return TeamStarts[FMath::RandRange(0, TeamStarts.Num() - 1)];
		}
	}
	return Super::ChoosePlayerStart_Implementation(Player);
}

void AArenaGameMode::OnCharacterKilled(AArenaCharacterBase* Victim,
	AActor* Killer, const TArray<AActor*>& Assisters)
{
	AArenaGameState* GS = GetGameState<AArenaGameState>();
	if (!GS || GS->MatchPhase != EArenaMatchPhase::InProgress)
	{
		return; // Warmup kills do not count.
	}

	AArenaPlayerState* VictimPS = Victim->GetPlayerState<AArenaPlayerState>();
	if (VictimPS)
	{
		VictimPS->Deaths++;
	}

	const bool bVictimIsHero = VictimPS != nullptr;
	const float KillPoints = bVictimIsHero ? 100.f : 10.f;

	if (AArenaPlayerState* KillerPS = Cast<AArenaPlayerState>(Killer))
	{
		if (KillerPS != VictimPS &&
			!UArenaAbilityLibrary::AreFriendly(KillerPS, Victim))
		{
			KillerPS->Kills++;
			KillerPS->SetScore(KillerPS->GetScore() + KillPoints);
			GS->AddTeamScore(KillerPS->TeamId, 1);
		}
	}

	for (AActor* Assister : Assisters)
	{
		if (AArenaPlayerState* AssistPS = Cast<AArenaPlayerState>(Assister))
		{
			AssistPS->Assists++;
			AssistPS->SetScore(AssistPS->GetScore() + KillPoints * 0.5f);
		}
	}

	CheckMatchEnd();
}

void AArenaGameMode::BeginPlay()
{
	Super::BeginPlay();
	SetMatchPhase(EArenaMatchPhase::Warmup, WarmupDuration);
}

void AArenaGameMode::SetMatchPhase(EArenaMatchPhase NewPhase, float Duration)
{
	AArenaGameState* GS = GetGameState<AArenaGameState>();
	GS->MatchPhase = NewPhase;
	GS->PhaseEndTime = GetWorld()->GetTimeSeconds() + Duration;
	GetWorldTimerManager().SetTimer(PhaseTimer, this,
		&AArenaGameMode::OnPhaseTimerExpired, Duration, false);
}

void AArenaGameMode::OnPhaseTimerExpired()
{
	AArenaGameState* GS = GetGameState<AArenaGameState>();
	switch (GS->MatchPhase)
	{
	case EArenaMatchPhase::Warmup:
		// Fresh start: reset scores and respawn everyone.
		GS->TeamScores.Reset();
		for (APlayerState* PS : GS->PlayerArray)
		{
			if (AArenaPlayerState* APS = Cast<AArenaPlayerState>(PS))
			{
				APS->Kills = APS->Deaths = APS->Assists = 0;
				APS->SetScore(0.f);
				if (AController* C = APS->GetOwningController())
				{
					if (APawn* P = C->GetPawn()) { C->UnPossess(); P->Destroy(); }
					RestartPlayer(C);
				}
			}
		}
		SetMatchPhase(EArenaMatchPhase::InProgress, MatchDuration);
		break;

	case EArenaMatchPhase::InProgress:
		EndMatch();   // time is up
		break;

	case EArenaMatchPhase::Ended:
		// Chapter 22: travel back to the lobby.
		break;
	}
}

void AArenaGameMode::CheckMatchEnd()
{
	AArenaGameState* GS = GetGameState<AArenaGameState>();
	if (GS->MatchPhase != EArenaMatchPhase::InProgress || bCoop)
	{
		return;
	}

	for (int32 Team = 1; Team < GS->TeamScores.Num(); ++Team)
	{
		if (GS->TeamScores[Team] >= ScoreLimit)
		{
			EndMatch();
			return;
		}
	}
}

void AArenaGameMode::EndMatch()
{
	AArenaGameState* GS = GetGameState<AArenaGameState>();

	// The team with the highest score wins; a tie has no winner.
	GS->WinningTeam = 0;
	int32 BestScore = -1;
	for (int32 Team = 1; Team < GS->TeamScores.Num(); ++Team)
	{
		if (GS->TeamScores[Team] > BestScore)
		{
			BestScore = GS->TeamScores[Team];
			GS->WinningTeam = Team;
		}
		else if (GS->TeamScores[Team] == BestScore)
		{
			GS->WinningTeam = 0;
		}
	}

	// Stop all abilities for every player.
	if (MatchOverEffect)
	{
		for (APlayerState* PS : GS->PlayerArray)
		{
			if (AArenaPlayerState* APS = Cast<AArenaPlayerState>(PS))
			{
				UAbilitySystemComponent* ASC = APS->GetAbilitySystemComponent();
				ASC->ApplyGameplayEffectSpecToSelf(*ASC->MakeOutgoingSpec(
					MatchOverEffect, 1.f, ASC->MakeEffectContext()).Data.Get());
			}
		}
	}

	SetMatchPhase(EArenaMatchPhase::Ended, EndDuration);
}

UClass* AArenaGameMode::GetDefaultPawnClassForController_Implementation(
	AController* InController)
{
	if (const AArenaPlayerState* PS = InController
			? InController->GetPlayerState<AArenaPlayerState>() : nullptr)
	{
		if (PS->HeroData && PS->HeroData->PawnClass)
		{
			return PS->HeroData->PawnClass;
		}
	}
	return Super::GetDefaultPawnClassForController_Implementation(InController);
}
