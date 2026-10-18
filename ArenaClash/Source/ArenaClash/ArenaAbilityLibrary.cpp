#include "ArenaAbilityLibrary.h"
#include "AbilitySystemComponent.h"

bool UArenaAbilityLibrary::GetCooldownRemaining(
	UAbilitySystemComponent* ASC, FGameplayTag CooldownTag,
	float& TimeRemaining, float& Duration)
{
	TimeRemaining = 0.f;
	Duration = 0.f;

	if (!ASC || !CooldownTag.IsValid())
	{
		return false;
	}

	const FGameplayEffectQuery Query =
		FGameplayEffectQuery::MakeQuery_MatchAnyOwningTags(
			CooldownTag.GetSingleTagContainer());

	const TArray<TPair<float, float>> Times =
		ASC->GetActiveEffectsTimeRemainingAndDuration(Query);

	for (const TPair<float, float>& Pair : Times)
	{
		if (Pair.Key > TimeRemaining)
		{
			TimeRemaining = Pair.Key;
			Duration = Pair.Value;
		}
	}
	return TimeRemaining > 0.f;
}
