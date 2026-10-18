#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "GameplayEffectTypes.h"
#include "ArenaProjectile.generated.h"

class USphereComponent;
class UProjectileMovementComponent;

UCLASS()
class ARENACLASH_API AArenaProjectile : public AActor
{
	GENERATED_BODY()

public:
	AArenaProjectile();

	// Set by the ability before the projectile finishes spawning.
	UPROPERTY(BlueprintReadWrite, Category = "Projectile",
			  meta = (ExposeOnSpawn = true))
	FGameplayEffectSpecHandle DamageSpec;

protected:
	virtual void BeginPlay() override;

	UFUNCTION()
	void OnSphereOverlap(UPrimitiveComponent* OverlappedComponent,
						 AActor* OtherActor, UPrimitiveComponent* OtherComp,
						 int32 OtherBodyIndex, bool bFromSweep,
						 const FHitResult& SweepResult);

	UFUNCTION()
	void OnSphereHit(UPrimitiveComponent* HitComponent, AActor* OtherActor,
					 UPrimitiveComponent* OtherComp, FVector NormalImpulse,
					 const FHitResult& Hit);

	// Called on the server when the projectile should explode.
	UFUNCTION(BlueprintNativeEvent, Category = "Projectile")
	void Explode(const FVector& Location);

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<USphereComponent> Sphere;

	UPROPERTY(VisibleAnywhere)
	TObjectPtr<UProjectileMovementComponent> Movement;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	float LifeSeconds = 4.f;

	UPROPERTY(EditDefaultsOnly, Category = "Projectile",
			  meta = (Categories = "GameplayCue"))
	FGameplayTag ImpactCueTag;
};
