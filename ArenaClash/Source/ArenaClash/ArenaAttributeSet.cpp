#include "ArenaAttributeSet.h"
#include "GameplayEffectExtension.h"
#include "Net/UnrealNetwork.h"
#include "ArenaCharacterBase.h"
#include "ArenaGameplayTags.h"

UArenaAttributeSet::UArenaAttributeSet()
{
}

void UArenaAttributeSet::GetLifetimeReplicatedProps(
	TArray<FLifetimeProperty>& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME_CONDITION_NOTIFY(UArenaAttributeSet, Health,      COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UArenaAttributeSet, MaxHealth,   COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UArenaAttributeSet, Mana,        COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UArenaAttributeSet, MaxMana,     COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UArenaAttributeSet, Armor,       COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UArenaAttributeSet, AttackPower, COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UArenaAttributeSet, MoveSpeed,   COND_None, REPNOTIFY_Always);
	DOREPLIFETIME_CONDITION_NOTIFY(UArenaAttributeSet, Shield,      COND_None, REPNOTIFY_Always);
}

void UArenaAttributeSet::PreAttributeChange(
	const FGameplayAttribute& Attribute, float& NewValue)
{
	Super::PreAttributeChange(Attribute, NewValue);

	if (Attribute == GetHealthAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxHealth());
	}
	else if (Attribute == GetManaAttribute())
	{
		NewValue = FMath::Clamp(NewValue, 0.f, GetMaxMana());
	}
	else if (Attribute == GetMoveSpeedAttribute() || Attribute == GetShieldAttribute())
	{
		NewValue = FMath::Max(NewValue, 0.f);
	}
}

void UArenaAttributeSet::PostGameplayEffectExecute(
	const FGameplayEffectModCallbackData& Data)
{
	Super::PostGameplayEffectExecute(Data);

	if (Data.EvaluatedData.Attribute == GetIncomingDamageAttribute())
	{
		const float Damage = GetIncomingDamage();
		SetIncomingDamage(0.f);

		UAbilitySystemComponent* TargetASC = &Data.Target;
		if (Damage <= 0.f ||
			TargetASC->HasMatchingGameplayTag(ArenaTags::State_Dead))
		{
			return;
		}

		float Remaining = Damage;

		const float CurrentShield = GetShield();
		if (CurrentShield > 0.f)
		{
			const float Absorbed = FMath::Min(CurrentShield, Remaining);
			SetShield(CurrentShield - Absorbed);
			Remaining -= Absorbed;
		}

		if (Remaining > 0.f)
		{
			SetHealth(FMath::Clamp(GetHealth() - Remaining, 0.f, GetMaxHealth()));
		}

		if (GetHealth() <= 0.f)
		{
			AActor* Killer =
				Data.EffectSpec.GetContext().GetOriginalInstigator();

			if (AArenaCharacterBase* Victim = Cast<AArenaCharacterBase>(
					TargetASC->GetAvatarActor()))
			{
				Victim->Die(Killer);
			}
		}
		return;
	}

	if (Data.EvaluatedData.Attribute == GetHealthAttribute())
	{
		SetHealth(FMath::Clamp(GetHealth(), 0.f, GetMaxHealth()));
	}
	else if (Data.EvaluatedData.Attribute == GetManaAttribute())
	{
		SetMana(FMath::Clamp(GetMana(), 0.f, GetMaxMana()));
	}
}

void UArenaAttributeSet::OnRep_Health(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaAttributeSet, Health, OldValue);
}

void UArenaAttributeSet::OnRep_MaxHealth(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaAttributeSet, MaxHealth, OldValue);
}

void UArenaAttributeSet::OnRep_Mana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaAttributeSet, Mana, OldValue);
}

void UArenaAttributeSet::OnRep_MaxMana(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaAttributeSet, MaxMana, OldValue);
}

void UArenaAttributeSet::OnRep_Armor(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaAttributeSet, Armor, OldValue);
}

void UArenaAttributeSet::OnRep_AttackPower(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaAttributeSet, AttackPower, OldValue);
}

void UArenaAttributeSet::OnRep_MoveSpeed(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaAttributeSet, MoveSpeed, OldValue);
}

void UArenaAttributeSet::OnRep_Shield(const FGameplayAttributeData& OldValue)
{
	GAMEPLAYATTRIBUTE_REPNOTIFY(UArenaAttributeSet, Shield, OldValue);
}
