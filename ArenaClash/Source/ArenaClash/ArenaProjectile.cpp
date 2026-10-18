#include "ArenaProjectile.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"
#include "GameFramework/ProjectileMovementComponent.h"

AArenaProjectile::AArenaProjectile()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(20.f);
	Sphere->SetCollisionProfileName(TEXT("Projectile"));
	RootComponent = Sphere;

	Movement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("Movement"));
	Movement->InitialSpeed = 2000.f;
	Movement->MaxSpeed = 2000.f;
	Movement->ProjectileGravityScale = 0.f;
}

void AArenaProjectile::BeginPlay()
{
	Super::BeginPlay();
	SetLifeSpan(LifeSeconds);

	if (HasAuthority())
	{
		Sphere->OnComponentBeginOverlap.AddDynamic(
			this, &AArenaProjectile::OnSphereOverlap);
		Sphere->OnComponentHit.AddDynamic(
			this, &AArenaProjectile::OnSphereHit);
	}
}

void AArenaProjectile::OnSphereOverlap(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, int32, bool, const FHitResult&)
{
	if (!OtherActor || OtherActor == GetInstigator())
	{
		return;
	}

	UAbilitySystemComponent* TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

	if (TargetASC && DamageSpec.IsValid())
	{
		TargetASC->ApplyGameplayEffectSpecToSelf(*DamageSpec.Data.Get());
	}

	Explode(GetActorLocation());
}

void AArenaProjectile::OnSphereHit(UPrimitiveComponent*, AActor* OtherActor,
	UPrimitiveComponent*, FVector, const FHitResult& Hit)
{
	// Hit a wall or the floor.
	Explode(Hit.ImpactPoint);
}

void AArenaProjectile::Explode_Implementation(const FVector& Location)
{
	if (ImpactCueTag.IsValid())
	{
		UAbilitySystemComponent* SourceASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(GetInstigator());
		if (SourceASC)
		{
			FGameplayCueParameters Params;
			Params.Location = Location;
			Params.Instigator = GetInstigator();
			Params.EffectCauser = this;
			SourceASC->ExecuteGameplayCue(ImpactCueTag, Params);
		}
	}

	Destroy();
}
