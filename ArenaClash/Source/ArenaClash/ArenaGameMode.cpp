#include "ArenaGameMode.h"
#include "ArenaGameState.h"
#include "ArenaPlayerController.h"
#include "ArenaPlayerState.h"
#include "GameFramework/GameStateBase.h"

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
