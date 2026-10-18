#include "ArenaAbilityLibrary.h"
#include "AbilitySystemComponent.h"
#include "ArenaCharacterBase.h"
#include "ArenaPlayerState.h"

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

uint8 UArenaAbilityLibrary::GetTeamIdOf(const AActor* Actor)
{
	if (const AArenaCharacterBase* Character = Cast<AArenaCharacterBase>(Actor))
	{
		return Character->GetTeamId();
	}
	if (const AArenaPlayerState* PS = Cast<AArenaPlayerState>(Actor))
	{
		return PS->TeamId;
	}
	return 0;
}

bool UArenaAbilityLibrary::AreFriendly(const AActor* A, const AActor* B)
{
	const uint8 TeamA = GetTeamIdOf(A);
	return TeamA != 0 && TeamA == GetTeamIdOf(B);
}
