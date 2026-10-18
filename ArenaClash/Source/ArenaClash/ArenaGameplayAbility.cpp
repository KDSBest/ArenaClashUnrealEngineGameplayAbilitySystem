#include "ArenaGameplayAbility.h"
#include "ArenaGameplayTags.h"

UArenaGameplayAbility::UArenaGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// No ability may be used while dead or stunned.
	ActivationBlockedTags.AddTag(ArenaTags::State_Dead);
	ActivationBlockedTags.AddTag(ArenaTags::State_Debuff_Stun);
}
