#include "ArenaGameplayAbility.h"
#include "ArenaGameplayTags.h"

UArenaGameplayAbility::UArenaGameplayAbility()
{
	InstancingPolicy = EGameplayAbilityInstancingPolicy::InstancedPerActor;
	NetExecutionPolicy = EGameplayAbilityNetExecutionPolicy::LocalPredicted;

	// No ability may be used while dead or stunned.
	ActivationBlockedTags.AddTag(ArenaTags::State_Dead);
	ActivationBlockedTags.AddTag(ArenaTags::State_Debuff_Stun);
	ActivationBlockedTags.AddTag(ArenaTags::State_MatchOver);
}

const FGameplayTagContainer* UArenaGameplayAbility::GetCooldownTags() const
{
	FGameplayTagContainer* MutableTags =
		const_cast<FGameplayTagContainer*>(&TempCooldownTags);
	MutableTags->Reset();

	if (const FGameplayTagContainer* ParentTags = Super::GetCooldownTags())
	{
		MutableTags->AppendTags(*ParentTags);
	}
	MutableTags->AppendTags(CooldownTags);
	return MutableTags;
}

void UArenaGameplayAbility::ApplyCooldown(
	const FGameplayAbilitySpecHandle Handle,
	const FGameplayAbilityActorInfo* ActorInfo,
	const FGameplayAbilityActivationInfo ActivationInfo) const
{
	UGameplayEffect* CooldownGE = GetCooldownGameplayEffect();
	if (!CooldownGE)
	{
		return;
	}

	FGameplayEffectSpecHandle Spec =
		MakeOutgoingGameplayEffectSpec(CooldownGE->GetClass(), GetAbilityLevel());
	if (!Spec.IsValid())
	{
		return;
	}

	Spec.Data->DynamicGrantedTags.AppendTags(CooldownTags);
	Spec.Data->SetSetByCallerMagnitude(
		ArenaTags::Data_Cooldown,
		CooldownDuration.GetValueAtLevel(GetAbilityLevel()));

	ApplyGameplayEffectSpecToOwner(Handle, ActorInfo, ActivationInfo, Spec);
}
