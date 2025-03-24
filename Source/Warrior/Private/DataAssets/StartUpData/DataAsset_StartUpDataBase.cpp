// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/StartUpData/DataAsset_StartUpDataBase.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"


void UDataAsset_StartUpDataBase::GiveToAbilitySystemComponent(UWarriorAbilitySystemComponent* InASCToGive, int32 ApplyLevel)
{
	check(InASCToGive);

	GrantAbilities(ActiveteOnGivenAbilities, InASCToGive, ApplyLevel);
	GrantAbilities(ReactiveAbilities, InASCToGive, ApplyLevel);

	for (const auto& Effect : StartUpGameplayEffects)
	{
		if (!Effect)
			continue;

		InASCToGive->ApplyGameplayEffectToSelf(Effect->GetDefaultObject<UGameplayEffect>(), ApplyLevel, InASCToGive->MakeEffectContext());
	}
}

void UDataAsset_StartUpDataBase::GrantAbilities(const TArray<TSubclassOf<UWarriorGameplayAbility>>& AbilitiesToGive, UWarriorAbilitySystemComponent* InASC, int32 ApplyLevel)
{
	if (AbilitiesToGive.IsEmpty())
		return;

	for (const auto& Ability : AbilitiesToGive)
	{
		if (!Ability)
			continue;

		FGameplayAbilitySpec AbilitySpec(Ability);
		AbilitySpec.SourceObject = InASC->GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;

		InASC->GiveAbility(AbilitySpec);
	}
}
