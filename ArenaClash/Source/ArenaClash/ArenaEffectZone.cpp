#include "ArenaEffectZone.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "Components/SphereComponent.h"

AArenaEffectZone::AArenaEffectZone()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->InitSphereRadius(100.f);
	Sphere->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	RootComponent = Sphere;
}

void AArenaEffectZone::NotifyActorBeginOverlap(AActor* OtherActor)
{
	Super::NotifyActorBeginOverlap(OtherActor);

	if (!HasAuthority() || !EffectClass)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);
	if (!TargetASC)
	{
		return;
	}

	FGameplayEffectContextHandle Context = TargetASC->MakeEffectContext();
	Context.AddSourceObject(this);

	const FGameplayEffectSpecHandle Spec =
		TargetASC->MakeOutgoingSpec(EffectClass, 1.f, Context);
	if (!Spec.IsValid())
	{
		return;
	}

	const FActiveGameplayEffectHandle Handle =
		TargetASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());

	if (bRemoveOnExit && Handle.IsValid())
	{
		ActiveEffects.Add(TargetASC, Handle);
	}

	if (bDestroyOnUse)
	{
		Destroy();
	}
}

void AArenaEffectZone::NotifyActorEndOverlap(AActor* OtherActor)
{
	Super::NotifyActorEndOverlap(OtherActor);

	if (!HasAuthority() || !bRemoveOnExit)
	{
		return;
	}

	UAbilitySystemComponent* TargetASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(OtherActor);

	FActiveGameplayEffectHandle Handle;
	if (TargetASC && ActiveEffects.RemoveAndCopyValue(TargetASC, Handle))
	{
		TargetASC->RemoveActiveGameplayEffect(Handle);
	}
}
