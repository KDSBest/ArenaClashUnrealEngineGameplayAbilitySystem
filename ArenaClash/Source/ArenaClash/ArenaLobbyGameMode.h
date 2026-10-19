#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaLobbyGameMode.generated.h"

class UArenaHeroData;

UCLASS()
class ARENACLASH_API AArenaLobbyGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArenaLobbyGameMode();

	// Called by the host's UI.
	UFUNCTION(BlueprintCallable, Category = "Lobby")
	void StartMatch(bool bCoopMode);

protected:
	virtual void PostLogin(APlayerController* NewPlayer) override;

	// Players walk around the lobby as their chosen hero.
	virtual UClass* GetDefaultPawnClassForController_Implementation(AController* InController) override;

	UPROPERTY(EditDefaultsOnly, Category = "Lobby")
	TObjectPtr<UArenaHeroData> DefaultHero;
};
