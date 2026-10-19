#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "ArenaPlayerController.generated.h"

class UInputMappingContext;
class UArenaHeroData;
class UAbilitySystemComponent;

UCLASS()
class ARENACLASH_API AArenaPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	UFUNCTION(BlueprintCallable, Server, Reliable, Category = "Hero")
	void ServerSelectHero(UArenaHeroData* Hero);

	// Called on the owning client when the hero's ASC is ready
	// (after spawn and after every respawn). Builds/updates the HUD.
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnHeroReady(UAbilitySystemComponent* AbilitySystem);

	UFUNCTION(Client, Unreliable)
	void ClientShowDamageNumber(AActor* Target, float Amount, bool bCritical);


protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void ShowDamageNumber(AActor* Target, float Amount, bool bCritical);
	virtual void BeginPlay() override;

	// After seamless travel, a listen server's new controller receives its
	// local player after BeginPlay. Local setup waits for both.
	virtual void ReceivedPlayer() override;
	void TryLocalSetup();

	// Called once on the owning machine when the controller is ready for
	// local UI (menus, lobby).
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void OnLocalControllerReady();

	bool bLocalSetupDone = false;

	UPROPERTY(EditDefaultsOnly, Category = "Input")
	TObjectPtr<UInputMappingContext> DefaultMappingContext;
};
