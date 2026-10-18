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

	bool IsFriendlyFireEnabled() const { return bFriendlyFire; }

protected:
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	float RespawnDelay = 5.f;

	virtual void PostLogin(APlayerController* NewPlayer) override;

	// true: everyone on team 1 against AI; false: two teams.
	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	bool bCoop = true;

	UPROPERTY(EditDefaultsOnly, Category = "Arena")
	bool bFriendlyFire = false;
};
