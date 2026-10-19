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
}

void AArenaEnemyCharacter::Die(AActor* Killer)
{
	Super::Die(Killer);
	SetLifeSpan(5.f);
}
