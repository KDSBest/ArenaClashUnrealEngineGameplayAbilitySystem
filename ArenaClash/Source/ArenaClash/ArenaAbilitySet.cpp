#include "ArenaAbilitySet.h"
#include "AbilitySystemComponent.h"
#include "ArenaGameplayAbility.h"

void UArenaAbilitySet::GiveToAbilitySystem(UAbilitySystemComponent* ASC,
	FArenaAbilitySetHandles* OutHandles) const
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FArenaAbilitySet_Ability& Entry : Abilities)
	{
		if (!Entry.Ability)
		{
			continue;
		}

		FGameplayAbilitySpec Spec(Entry.Ability, Entry.Level);

		const FGameplayTag InputTag = Entry.InputTagOverride.IsValid()
			? Entry.InputTagOverride
			: Entry.Ability->GetDefaultObject<UArenaGameplayAbility>()->InputTag;
		Spec.GetDynamicSpecSourceTags().AddTag(InputTag);

		const FGameplayAbilitySpecHandle Handle = ASC->GiveAbility(Spec);
		if (OutHandles)
		{
			OutHandles->Abilities.Add(Handle);
		}
	}

	for (const TSubclassOf<UGameplayEffect>& EffectClass : Effects)
	{
		if (!EffectClass)
		{
			continue;
		}

		const FGameplayEffectSpecHandle Spec =
			ASC->MakeOutgoingSpec(EffectClass, 1.f, ASC->MakeEffectContext());
		const FActiveGameplayEffectHandle Handle =
			ASC->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());

		if (OutHandles)
		{
			OutHandles->Effects.Add(Handle);
		}
	}
}

void FArenaAbilitySetHandles::TakeFromAbilitySystem(UAbilitySystemComponent* ASC)
{
	if (!ASC || !ASC->IsOwnerActorAuthoritative())
	{
		return;
	}

	for (const FGameplayAbilitySpecHandle& Handle : Abilities)
	{
		ASC->ClearAbility(Handle);
	}
	for (const FActiveGameplayEffectHandle& Handle : Effects)
	{
		ASC->RemoveActiveGameplayEffect(Handle);
	}

	Abilities.Reset();
	Effects.Reset();
}
