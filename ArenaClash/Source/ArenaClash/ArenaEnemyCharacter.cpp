#include "ArenaEnemyCharacter.h"
#include "AbilitySystemComponent.h"

AArenaEnemyCharacter::AArenaEnemyCharacter()
{
	EnemyAbilitySystemComponent =
		CreateDefaultSubobject<UAbilitySystemComponent>(TEXT("AbilitySystemComponent"));
	EnemyAbilitySystemComponent->SetIsReplicated(true);
	EnemyAbilitySystemComponent->SetReplicationMode(EGameplayEffectReplicationMode::Minimal);
}

void AArenaEnemyCharacter::BeginPlay()
{
	Super::BeginPlay();

	InitAbilityActorInfo();
}

void AArenaEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent = EnemyAbilitySystemComponent;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
}
