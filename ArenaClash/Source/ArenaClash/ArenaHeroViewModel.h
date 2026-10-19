#pragma once

#include "CoreMinimal.h"
#include "MVVMViewModelBase.h"
#include "GameplayEffectTypes.h"
#include "ArenaHeroViewModel.generated.h"

class UAbilitySystemComponent;

UCLASS(BlueprintType)
class ARENACLASH_API UArenaHeroViewModel : public UMVVMViewModelBase
{
	GENERATED_BODY()

public:
	// Connects the ViewModel to an ASC. Safe to call again on respawn.
	UFUNCTION(BlueprintCallable)
	void Initialize(UAbilitySystemComponent* InASC);

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetHealthPercent() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetManaPercent() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	float GetShieldPercent() const;

	UFUNCTION(BlueprintPure, FieldNotify)
	FText GetHealthText() const;

private:
	void OnAttributeChanged(const FOnAttributeChangeData& Data);
	void BindAttribute(const FGameplayAttribute& Attribute);

	TWeakObjectPtr<UAbilitySystemComponent> ASC;

	float Health = 0.f, MaxHealth = 1.f;
	float Mana = 0.f, MaxMana = 1.f;
	float Shield = 0.f;
};
