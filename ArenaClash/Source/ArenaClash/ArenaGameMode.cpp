#include "ArenaGameMode.h"
#include "ArenaGameState.h"
#include "ArenaPlayerController.h"
#include "ArenaPlayerState.h"

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
