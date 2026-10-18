#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ActiveGameplayEffectHandle.h"
#include "ArenaEffectZone.generated.h"

class USphereComponent;
class UGameplayEffect;
class UAbilitySystemComponent;

UCLASS()
class ARENACLASH_API AArenaEffectZone : public AActor
{
	GENERATED_BODY()

public:
	AArenaEffectZone();

protected:
	virtual void NotifyActorBeginOverlap(AActor* OtherActor) override;
	virtual void NotifyActorEndOverlap(AActor* OtherActor) override;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	// The effect to apply to characters that enter.
	UPROPERTY(EditAnywhere, Category = "Effect")
	TSubclassOf<UGameplayEffect> EffectClass;

	// Destroy this actor after it has been used once (pickups).
	UPROPERTY(EditAnywhere, Category = "Effect")
	bool bDestroyOnUse = false;

	// Remove infinite effects when the character leaves (zones).
	UPROPERTY(EditAnywhere, Category = "Effect")
	bool bRemoveOnExit = false;

private:
	TMap<TWeakObjectPtr<UAbilitySystemComponent>, FActiveGameplayEffectHandle>
		ActiveEffects;
};
