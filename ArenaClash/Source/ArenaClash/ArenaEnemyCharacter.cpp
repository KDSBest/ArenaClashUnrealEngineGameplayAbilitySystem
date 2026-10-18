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
	ApplyDefaultAttributes();
	GrantStartupAbilities();
}

void AArenaEnemyCharacter::InitAbilityActorInfo()
{
	AbilitySystemComponent = EnemyAbilitySystemComponent;
	AbilitySystemComponent->InitAbilityActorInfo(this, this);
	BindAttributeCallbacks();
}
