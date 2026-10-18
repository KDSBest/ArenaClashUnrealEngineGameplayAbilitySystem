#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ArenaNetDemo.generated.h"

class UStaticMeshComponent;
class UPointLightComponent;

UCLASS()
class ARENACLASH_API AArenaNetDemo : public AActor
{
	GENERATED_BODY()

public:
	AArenaNetDemo();

	virtual void GetLifetimeReplicatedProps(
		TArray<FLifetimeProperty>& OutLifetimeProps) const override;

	// Server only: switch the light on or off.
	void SetLightOn(bool bNewLightOn);

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnRep_LightOn();

	void ToggleLight();

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UStaticMeshComponent> Mesh;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UPointLightComponent> Light;

	UPROPERTY(ReplicatedUsing = OnRep_LightOn)
	bool bLightOn = false;

	FTimerHandle ToggleTimer;
};
