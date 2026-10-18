#include "ArenaPlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"

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
