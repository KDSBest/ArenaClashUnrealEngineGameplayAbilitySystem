#pragma once

#include "CoreMinimal.h"
#include "GameplayEffectExecutionCalculation.h"
#include "ArenaDamageExecution.generated.h"

UCLASS()
class ARENACLASH_API UArenaDamageExecution
	: public UGameplayEffectExecutionCalculation
{
	GENERATED_BODY()

public:
	UArenaDamageExecution();

	virtual void Execute_Implementation(
		const FGameplayEffectCustomExecutionParameters& ExecutionParams,
		FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const override;
};
