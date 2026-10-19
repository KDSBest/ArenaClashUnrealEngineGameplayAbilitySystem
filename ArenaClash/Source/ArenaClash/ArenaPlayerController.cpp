#include "ArenaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ArenaPlayerState.h"
#include "ArenaGameState.h"
#include "ArenaHeroData.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaLobbyGameMode.h"

void AArenaPlayerController::BeginPlay()
{
	Super::BeginPlay();
	TryLocalSetup();
}

void AArenaPlayerController::ReceivedPlayer()
{
	Super::ReceivedPlayer();
	TryLocalSetup();
}

void AArenaPlayerController::TryLocalSetup()
{
	// Input only matters on the machine of the player who owns
	// this controller.
	if (bLocalSetupDone || !HasActorBegunPlay() || !IsLocalController())
	{
		return;
	}
	bLocalSetupDone = true;

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}

	OnLocalControllerReady();
}

void AArenaPlayerController::ServerSelectHero_Implementation(UArenaHeroData* Hero)
{
	AArenaPlayerState* PS = GetPlayerState<AArenaPlayerState>();
	AArenaGameState* GS = GetWorld()->GetGameState<AArenaGameState>();
	const bool bInLobby = GetWorld()->GetAuthGameMode<AArenaLobbyGameMode>() != nullptr;
	if (!PS || !Hero)
	{
		return;
	}
	if (!bInLobby && (!GS || GS->MatchPhase != EArenaMatchPhase::Warmup))
	{
		return; // Heroes can only be changed in the lobby or during warmup.
	}

	PS->SetHeroData(Hero);

	// Respawn as the new hero.
	if (APawn* CurrentPawn = GetPawn())
	{
		UnPossess();
		CurrentPawn->Destroy();
	}
	GetWorld()->GetAuthGameMode()->RestartPlayer(this);
}

void AArenaPlayerController::ClientShowDamageNumber_Implementation(
	AActor* Target, float Amount, bool bCritical)
{
	ShowDamageNumber(Target, Amount, bCritical);
}
