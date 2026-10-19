#include "ArenaLobbyGameMode.h"
#include "ArenaPlayerState.h"
#include "ArenaPlayerController.h"
#include "ArenaHeroData.h"
#include "ArenaPlayerCharacter.h"

AArenaLobbyGameMode::AArenaLobbyGameMode()
{
	PlayerStateClass = AArenaPlayerState::StaticClass();
	PlayerControllerClass = AArenaPlayerController::StaticClass();
	bUseSeamlessTravel = true;
}

void AArenaLobbyGameMode::PostLogin(APlayerController* NewPlayer)
{
	// Before Super::PostLogin, which spawns the first pawn.
	AArenaPlayerState* PS = NewPlayer->GetPlayerState<AArenaPlayerState>();
	if (PS && !PS->HeroData)
	{
		PS->SetHeroData(DefaultHero);
	}

	Super::PostLogin(NewPlayer);
}

UClass* AArenaLobbyGameMode::GetDefaultPawnClassForController_Implementation(
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

void AArenaLobbyGameMode::StartMatch(bool bCoopMode)
{
	const FString Mode = bCoopMode ? TEXT("Coop") : TEXT("PvP");
	GetWorld()->ServerTravel(
		FString::Printf(TEXT("/Game/ArenaClash/Maps/L_Arena?Mode=%s?listen"), *Mode));
}
