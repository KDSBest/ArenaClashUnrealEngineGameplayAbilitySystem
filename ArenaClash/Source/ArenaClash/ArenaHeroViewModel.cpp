#include "ArenaHeroViewModel.h"
#include "AbilitySystemComponent.h"
#include "ArenaAttributeSet.h"

void UArenaHeroViewModel::Initialize(UAbilitySystemComponent* InASC)
{
	if (ASC.IsValid())
	{
		// Unbind from a previous ASC.
		for (const FGameplayAttribute& Attr :
			 { UArenaAttributeSet::GetHealthAttribute(),
			   UArenaAttributeSet::GetMaxHealthAttribute(),
			   UArenaAttributeSet::GetManaAttribute(),
			   UArenaAttributeSet::GetMaxManaAttribute(),
			   UArenaAttributeSet::GetShieldAttribute() })
		{
			ASC->GetGameplayAttributeValueChangeDelegate(Attr).RemoveAll(this);
		}
	}

	ASC = InASC;
	if (!InASC)
	{
		return;
	}

	BindAttribute(UArenaAttributeSet::GetHealthAttribute());
	BindAttribute(UArenaAttributeSet::GetMaxHealthAttribute());
	BindAttribute(UArenaAttributeSet::GetManaAttribute());
	BindAttribute(UArenaAttributeSet::GetMaxManaAttribute());
	BindAttribute(UArenaAttributeSet::GetShieldAttribute());
}

void UArenaHeroViewModel::BindAttribute(const FGameplayAttribute& Attribute)
{
	ASC->GetGameplayAttributeValueChangeDelegate(Attribute)
		.AddUObject(this, &UArenaHeroViewModel::OnAttributeChanged);

	// Push the current value once, so the UI is right from the start.
	FOnAttributeChangeData Initial;
	Initial.Attribute = Attribute;
	Initial.NewValue = ASC->GetNumericAttribute(Attribute);
	OnAttributeChanged(Initial);
}

void UArenaHeroViewModel::OnAttributeChanged(const FOnAttributeChangeData& Data)
{
	const FGameplayAttribute& A = Data.Attribute;
	if      (A == UArenaAttributeSet::GetHealthAttribute())    Health = Data.NewValue;
	else if (A == UArenaAttributeSet::GetMaxHealthAttribute()) MaxHealth = FMath::Max(Data.NewValue, 1.f);
	else if (A == UArenaAttributeSet::GetManaAttribute())      Mana = Data.NewValue;
	else if (A == UArenaAttributeSet::GetMaxManaAttribute())   MaxMana = FMath::Max(Data.NewValue, 1.f);
	else if (A == UArenaAttributeSet::GetShieldAttribute())    Shield = Data.NewValue;

	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthPercent);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetManaPercent);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetShieldPercent);
	UE_MVVM_BROADCAST_FIELD_VALUE_CHANGED(GetHealthText);
}

float UArenaHeroViewModel::GetHealthPercent() const { return Health / MaxHealth; }
float UArenaHeroViewModel::GetManaPercent() const   { return Mana / MaxMana; }
float UArenaHeroViewModel::GetShieldPercent() const { return Shield / MaxHealth; }

FText UArenaHeroViewModel::GetHealthText() const
{
	return FText::FromString(FString::Printf(TEXT("%d / %d"),
		FMath::CeilToInt(Health), FMath::RoundToInt(MaxHealth)));
}
