#include "BTTask_ActivateAbilityByTag.h"
#include "AIController.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystemComponent.h"
#include "BehaviorTree/BehaviorTreeComponent.h"

UBTTask_ActivateAbilityByTag::UBTTask_ActivateAbilityByTag()
{
	NodeName = TEXT("Activate Ability by Tag");
	bCreateNodeInstance = true; // we store per-AI state
}

EBTNodeResult::Type UBTTask_ActivateAbilityByTag::ExecuteTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	APawn* Pawn = OwnerComp.GetAIOwner()->GetPawn();
	UAbilitySystemComponent* ASC =
		UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(Pawn);
	if (!ASC)
	{
		return EBTNodeResult::Failed;
	}

	CachedOwnerComp = &OwnerComp;
	EndedHandle = ASC->OnAbilityEnded.AddWeakLambda(this,
		[this](const FAbilityEndedData& Data)
		{
			if (Data.AbilityThatEnded->AbilityTags.HasTagExact(AbilityTag) &&
				CachedOwnerComp.IsValid())
			{
				FinishLatentTask(*CachedOwnerComp, EBTNodeResult::Succeeded);
			}
		});

	if (!ASC->TryActivateAbilitiesByTag(AbilityTag.GetSingleTagContainer()))
	{
		ASC->OnAbilityEnded.Remove(EndedHandle);
		return EBTNodeResult::Failed;   // on cooldown, stunned, ...
	}
	return EBTNodeResult::InProgress;
}

EBTNodeResult::Type UBTTask_ActivateAbilityByTag::AbortTask(
	UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	if (UAbilitySystemComponent* ASC =
			UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(
				OwnerComp.GetAIOwner()->GetPawn()))
	{
		ASC->OnAbilityEnded.Remove(EndedHandle);
		FGameplayTagContainer Tags = AbilityTag.GetSingleTagContainer();
		ASC->CancelAbilities(&Tags);
	}
	return EBTNodeResult::Aborted;
}

FString UBTTask_ActivateAbilityByTag::GetStaticDescription() const
{
	return FString::Printf(TEXT("Activate %s"), *AbilityTag.ToString());
}
