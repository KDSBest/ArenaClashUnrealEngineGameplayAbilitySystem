#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "GameplayTagContainer.h"
#include "BTTask_ActivateAbilityByTag.generated.h"

class UGameplayAbility;

UCLASS()
class ARENACLASH_API UBTTask_ActivateAbilityByTag : public UBTTaskNode
{
	GENERATED_BODY()

public:
	UBTTask_ActivateAbilityByTag();

	UPROPERTY(EditAnywhere, Category = "Ability")
	FGameplayTag AbilityTag;

protected:
	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp,
											uint8* NodeMemory) override;
	virtual EBTNodeResult::Type AbortTask(UBehaviorTreeComponent& OwnerComp,
										  uint8* NodeMemory) override;
	virtual FString GetStaticDescription() const override;

private:
	TWeakObjectPtr<UBehaviorTreeComponent> CachedOwnerComp;
	FDelegateHandle EndedHandle;
};
