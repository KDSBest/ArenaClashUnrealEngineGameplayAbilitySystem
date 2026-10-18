#include "ArenaDamageExecution.h"
#include "AbilitySystemComponent.h"
#include "ArenaAttributeSet.h"
#include "ArenaGameplayTags.h"

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

	if (Damage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(FGameplayModifierEvaluatedData(
			UArenaAttributeSet::GetIncomingDamageAttribute(),
			EGameplayModOp::Additive, Damage));
	}
}
