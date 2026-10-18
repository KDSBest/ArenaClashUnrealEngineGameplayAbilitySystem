#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "ArenaAbilityTask_WaitLanded.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FArenaWaitLandedDelegate);

UCLASS()
class ARENACLASH_API UArenaAbilityTask_WaitLanded : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FArenaWaitLandedDelegate OnLanded;

	// Waits until the avatar character touches the ground.
	// Fires immediately if it is already on the ground.
	UFUNCTION(BlueprintCallable, Category = "Ability|Tasks",
			  meta = (HidePin = "OwningAbility",
					  DefaultToSelf = "OwningAbility",
					  BlueprintInternalUseOnly = "true"))
	static UArenaAbilityTask_WaitLanded* WaitLanded(UGameplayAbility* OwningAbility);

protected:
	virtual void Activate() override;
	virtual void OnDestroy(bool bInOwnerFinished) override;

private:
	UFUNCTION()
	void HandleLanded(const FHitResult& Hit);

	void Finish();
};
