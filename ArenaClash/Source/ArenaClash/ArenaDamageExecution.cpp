#include "ArenaDamageExecution.h"
#include "AbilitySystemComponent.h"
#include "ArenaAttributeSet.h"
#include "ArenaGameplayTags.h"
#include "ArenaAbilityLibrary.h"
#include "ArenaGameMode.h"
#include "ArenaClash.h"
#include "VisualLogger/VisualLogger.h"

struct FArenaDamageStatics
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower);
	DECLARE_ATTRIBUTE_CAPTUREDEF(Armor);

	FArenaDamageStatics()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArenaAttributeSet, AttackPower, Source, true);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UArenaAttributeSet, Armor, Target, false);
	}
};

static const FArenaDamageStatics& DamageStatics()
{
	static FArenaDamageStatics Statics;
	return Statics;
}

UArenaDamageExecution::UArenaDamageExecution()
{
	RelevantAttributesToCapture.Add(DamageStatics().AttackPowerDef);
	RelevantAttributesToCapture.Add(DamageStatics().ArmorDef);
}

void UArenaDamageExecution::Execute_Implementation(
	const FGameplayEffectCustomExecutionParameters& ExecutionParams,
	FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const FGameplayEffectSpec& Spec = ExecutionParams.GetOwningSpec();

	UAbilitySystemComponent* SourceASC = ExecutionParams.GetSourceAbilitySystemComponent();
	UAbilitySystemComponent* TargetASC = ExecutionParams.GetTargetAbilitySystemComponent();
	AActor* SourceActor = SourceASC ? SourceASC->GetAvatarActor() : nullptr;
	AActor* TargetActor = TargetASC ? TargetASC->GetAvatarActor() : nullptr;

	const AArenaGameMode* GM = TargetActor
		? TargetActor->GetWorld()->GetAuthGameMode<AArenaGameMode>()
		: nullptr;
	const bool bFriendlyFire = GM && GM->IsFriendlyFireEnabled();

	if (!bFriendlyFire && SourceActor != TargetActor &&
		UArenaAbilityLibrary::AreFriendly(SourceActor, TargetActor))
	{
		return; // No damage to teammates.
	}

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = Spec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = Spec.CapturedTargetTags.GetAggregatedTags();

	float AttackPower = 0.f;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
		DamageStatics().AttackPowerDef, EvalParams, AttackPower);
	AttackPower = FMath::Max(AttackPower, 0.f);

	// Base damage from the ability. 'false' = no warning if missing.
	const float BaseDamage = FMath::Max(
		Spec.GetSetByCallerMagnitude(ArenaTags::Data_Damage, false, 0.f), 0.f);

	// Every 10 points of AttackPower add 10% damage.
	float Damage = BaseDamage * (1.f + AttackPower / 100.f);

	FGameplayTagContainer AssetTags;
	Spec.GetAllAssetTags(AssetTags);

	// Physical damage is reduced by armor.
	if (AssetTags.HasTag(ArenaTags::Damage_Type_Physical))
	{
		float Armor = 0.f;
		ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(
			DamageStatics().ArmorDef, EvalParams, Armor);
		Armor = FMath::Max(Armor, 0.f);

		Damage *= 100.f / (100.f + Armor);
	}

	// Fire damage is 50% stronger against burning targets.
	if (AssetTags.HasTag(ArenaTags::Damage_Type_Fire) &&
		EvalParams.TargetTags &&
		EvalParams.TargetTags->HasTag(ArenaTags::State_Debuff_Burning))
	{
		Damage *= 1.5f;
	}

	// 10% chance of a critical hit for 150% damage.
	bool bCritical = false;
	if (FMath::FRand() < 0.1f)
	{
		Damage *= 1.5f;
		bCritical = true;
	}

	if (Damage > 0.f && bCritical)
	{
		// Must come before the damage: modifiers are processed in order.
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UArenaAttributeSet::GetIncomingCritAttribute(),
			EGameplayModOp::Additive, 1.f));
	}

	if (Damage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UArenaAttributeSet::GetIncomingDamageAttribute(),
			EGameplayModOp::Additive, Damage));
	}

	UE_VLOG(SourceActor, LogArena, Log, TEXT("Damage %.1f to %s%s"), Damage,
			*GetNameSafe(TargetActor), bCritical ? TEXT(" (critical)") : TEXT(""));
	if (TargetActor)
	{
		UE_VLOG_LOCATION(SourceActor, LogArena, Log, TargetActor->GetActorLocation(),
						 30.f, FColor::Red, TEXT("%.0f"), Damage);
	}
}
