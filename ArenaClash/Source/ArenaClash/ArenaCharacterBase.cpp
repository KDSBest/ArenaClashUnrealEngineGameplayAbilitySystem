#include "ArenaCharacterBase.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "AbilitySystemComponent.h"
#include "ArenaAttributeSet.h"
#include "ArenaGameplayAbility.h"
#include "ArenaGameplayTags.h"
#include "Components/CapsuleComponent.h"
#include "ArenaGameMode.h"

AArenaCharacterBase::AArenaCharacterBase()
{
	PrimaryActorTick.bCanEverTick = false;

	// Turn the character toward the direction it moves,
	// not toward where the camera looks.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	UCharacterMovementComponent* Movement = GetCharacterMovement();
	Movement->bOrientRotationToMovement = true;
	Movement->RotationRate = FRotator(0.f, 500.f, 0.f);
	Movement->JumpZVelocity = 700.f;
	Movement->MaxWalkSpeed = 500.f;
}

UAbilitySystemComponent* AArenaCharacterBase::GetAbilitySystemComponent() const
{
	return AbilitySystemComponent;
}

void AArenaCharacterBase::ApplyDefaultAttributes()
{
	if (!HasAuthority() || !AbilitySystemComponent || !GetDefaultAttributesEffect())
	{
		return;
	}

	FGameplayEffectContextHandle Context =
		AbilitySystemComponent->MakeEffectContext();
	Context.AddSourceObject(this);

	const FGameplayEffectSpecHandle Spec =
		AbilitySystemComponent->MakeOutgoingSpec(GetDefaultAttributesEffect(), 1.f, Context);

	if (Spec.IsValid())
	{
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	for (const TSubclassOf<UGameplayEffect>& EffectClass : StartupEffects)
	{
		if (!EffectClass)
		{
			continue;
		}

		// The hero's ASC survives respawns, so remove the old instance
		// before applying the effect again.
		AbilitySystemComponent->RemoveActiveGameplayEffectBySourceEffect(EffectClass, nullptr);

		const FGameplayEffectSpecHandle StartupSpec =
			AbilitySystemComponent->MakeOutgoingSpec(EffectClass, 1.f, Context);
		if (StartupSpec.IsValid())
		{
			AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*StartupSpec.Data.Get());
		}
	}
}

void AArenaCharacterBase::BindAttributeCallbacks()
{
	if (!AbilitySystemComponent)
	{
		return;
	}

	FOnGameplayAttributeValueChange& Delegate =
		AbilitySystemComponent->GetGameplayAttributeValueChangeDelegate(
			UArenaAttributeSet::GetMoveSpeedAttribute());

	// Avoid binding twice if InitAbilityActorInfo runs again.
	Delegate.RemoveAll(this);
	Delegate.AddUObject(this, &AArenaCharacterBase::OnMoveSpeedChanged);

	FOnGameplayEffectTagCountChanged& StunEvent =
		AbilitySystemComponent->RegisterGameplayTagEvent(
			ArenaTags::State_Debuff_Stun, EGameplayTagEventType::NewOrRemoved);
	StunEvent.RemoveAll(this);
	StunEvent.AddUObject(this, &AArenaCharacterBase::OnStunTagChanged);

	FOnGameplayEffectTagCountChanged& ShieldEvent =
		AbilitySystemComponent->RegisterGameplayTagEvent(
			ArenaTags::State_Buff_Shield, EGameplayTagEventType::NewOrRemoved);
	ShieldEvent.RemoveAll(this);
	ShieldEvent.AddUObject(this, &AArenaCharacterBase::OnShieldTagChanged);
}

void AArenaCharacterBase::OnMoveSpeedChanged(const FOnAttributeChangeData& Data)
{
	GetCharacterMovement()->MaxWalkSpeed = Data.NewValue;
}

void AArenaCharacterBase::GrantStartupAbilities()
{
	if (!HasAuthority() || !AbilitySystemComponent)
	{
		return;
	}

	for (const TSubclassOf<UArenaGameplayAbility>& AbilityClass : StartupAbilities)
	{
		if (!AbilityClass)
		{
			continue;
		}

		// Do not grant the same ability twice (e.g. after a respawn).
		if (AbilitySystemComponent->FindAbilitySpecFromClass(AbilityClass))
		{
			continue;
		}

		FGameplayAbilitySpec Spec(AbilityClass, 1);

		const UArenaGameplayAbility* CDO =
			AbilityClass->GetDefaultObject<UArenaGameplayAbility>();
		Spec.GetDynamicSpecSourceTags().AddTag(CDO->InputTag);

		AbilitySystemComponent->GiveAbility(Spec);
	}
}

void AArenaCharacterBase::Die(AActor* Killer)
{
	if (!HasAuthority() || IsDead())
	{
		return;
	}

	AbilitySystemComponent->CancelAllAbilities();

	if (DeathEffect)
	{
		const FGameplayEffectSpecHandle Spec =
			AbilitySystemComponent->MakeOutgoingSpec(
				DeathEffect, 1.f, AbilitySystemComponent->MakeEffectContext());
		AbilitySystemComponent->ApplyGameplayEffectSpecToSelf(*Spec.Data.Get());
	}

	if (AArenaGameMode* GM = GetWorld()->GetAuthGameMode<AArenaGameMode>())
	{
		TArray<AActor*> Assisters;
		const double Now = GetWorld()->GetTimeSeconds();
		for (const auto& Pair : RecentDamagers)
		{
			AActor* Damager = Pair.Key.Get();
			if (Damager && Damager != Killer && Now - Pair.Value < 10.0)
			{
				Assisters.Add(Damager);
			}
		}
		RecentDamagers.Reset();

		GM->OnCharacterKilled(this, Killer, Assisters);
	}
}

bool AArenaCharacterBase::IsDead() const
{
	return AbilitySystemComponent &&
		   AbilitySystemComponent->HasMatchingGameplayTag(ArenaTags::State_Dead);
}

void AArenaCharacterBase::OnStunTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (IsDead())
	{
		return;
	}

	UCharacterMovementComponent* Movement = GetCharacterMovement();

	if (NewCount > 0)
	{
		Movement->StopMovementImmediately();
		Movement->DisableMovement();

		// Stop everything the character is doing.
		FGameplayTagContainer IgnoreTags;
		IgnoreTags.AddTag(FGameplayTag::RequestGameplayTag("Ability.Passive"));
		AbilitySystemComponent->CancelAbilities(nullptr, &IgnoreTags);
	}
	else
	{
		Movement->SetMovementMode(MOVE_Walking);
	}
}

void AArenaCharacterBase::OnShieldTagChanged(const FGameplayTag Tag, int32 NewCount)
{
	if (NewCount == 0 && HasAuthority())
	{
		AbilitySystemComponent->SetNumericAttributeBase(
			UArenaAttributeSet::GetShieldAttribute(), 0.f);
	}
}

void AArenaCharacterBase::RecordDamageFrom(AActor* DamageInstigator)
{
	if (DamageInstigator)
	{
		RecentDamagers.Add(DamageInstigator, GetWorld()->GetTimeSeconds());
	}
}
