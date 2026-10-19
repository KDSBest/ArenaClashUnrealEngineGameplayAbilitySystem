#include "ArenaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "ArenaPlayerState.h"
#include "ArenaGameState.h"
#include "ArenaHeroData.h"
#include "GameFramework/GameModeBase.h"

void AArenaPlayerController::BeginPlay()
{
	Super::BeginPlay();

	// Input only matters on the machine of the player who owns
	// this controller.
	if (!IsLocalController())
	{
		return;
	}

	if (UEnhancedInputLocalPlayerSubsystem* Subsystem =
			ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(
				GetLocalPlayer()))
	{
		Subsystem->AddMappingContext(DefaultMappingContext, 0);
	}
}

void AArenaPlayerController::ServerSelectHero_Implementation(UArenaHeroData* Hero)
{
	AArenaPlayerState* PS = GetPlayerState<AArenaPlayerState>();
	AArenaGameState* GS = GetWorld()->GetGameState<AArenaGameState>();
	if (!PS || !Hero || !GS || GS->MatchPhase != EArenaMatchPhase::Warmup)
	{
		return; // Heroes can only be changed during warmup.
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
