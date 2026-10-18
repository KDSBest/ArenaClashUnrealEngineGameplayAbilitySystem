#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "ArenaGameMode.generated.h"

UCLASS()
class ARENACLASH_API AArenaGameMode : public AGameModeBase
{
	GENERATED_BODY()

public:
	AArenaGameMode();

	void RequestRespawn(AController* Controller);

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	float RespawnDelay = 5.f;
};
