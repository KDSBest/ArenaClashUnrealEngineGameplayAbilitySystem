#include "ArenaGameMode.h"
#include "ArenaGameState.h"
#include "ArenaPlayerController.h"
#include "ArenaPlayerState.h"
#include "GameFramework/GameStateBase.h"
#include "GameFramework/PlayerStart.h"
#include "EngineUtils.h"

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
	Super::PostLogin(NewPlayer);

	AArenaPlayerState* PS = NewPlayer->GetPlayerState<AArenaPlayerState>();
	if (!PS)
	{
		return;
	}

	if (bCoop)
	{
		PS->TeamId = 1;
		return;
	}

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
