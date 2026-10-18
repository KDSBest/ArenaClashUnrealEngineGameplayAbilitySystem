#include "ArenaNetDemo.h"
#include "Components/PointLightComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Net/UnrealNetwork.h"

AArenaNetDemo::AArenaNetDemo()
{
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	RootComponent = Mesh;

	Light = CreateDefaultSubobject<UPointLightComponent>(TEXT("Light"));
	Light->SetupAttachment(Mesh);
	Light->SetRelativeLocation(FVector(0.f, 0.f, 150.f));
	Light->SetVisibility(false);
}

void AArenaNetDemo::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AArenaNetDemo, bLightOn);
}

void AArenaNetDemo::BeginPlay()
{
	Super::BeginPlay();

	if (HasAuthority())
	{
		GetWorldTimerManager().SetTimer(
			ToggleTimer, this, &AArenaNetDemo::ToggleLight, 2.f, true);
	}
}

void AArenaNetDemo::ToggleLight()
{
	SetLightOn(!bLightOn);
}

void AArenaNetDemo::SetLightOn(bool bNewLightOn)
{
	bLightOn = bNewLightOn;

	// OnRep functions are not called automatically on the server
	// in C++, so we call it ourselves.
	OnRep_LightOn();
}

void AArenaNetDemo::OnRep_LightOn()
{
	Light->SetVisibility(bLightOn);
}
