#include "ArenaEnemyCharacter.h"
#include "AbilitySystemComponent.h"
#include "ArenaAttributeSet.h"

AArenaEnemyCharacter::AArenaEnemyCharacter()
{
	EnemyAbilitySystemComponent =
		CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	EnemyAbilitySystemComponent->SetIsReplicated(true);
	EnemyAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);

	EnemyAttributeSet = CreateDefaultSubobject<UArenaAttributeSet>(TEXT("AttributeSet"));

	// Set here, not only in BeginPlay: on clients the attribute OnReps can
	// arrive before BeginPlay and need GetAbilitySystemComponent().
	AbilitySystemComponent = EnemyAbilitySystemComponent;
}

void AArenaEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
	ApplyDefaultAttributes(EnemyLevel);
	GrantStartupAbilities();
}

void AArenaEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent = EnemyAbilitySystemComponent;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	BindAttributeCallbacks();
	OnAbilitySystemReady(AbilitySystemComponent);
}

void AArenaEnemyCharacter::Die(AActor* Killer)
{
	Super::Die(Killer);
	SetLifeSpan(5.f);
}
