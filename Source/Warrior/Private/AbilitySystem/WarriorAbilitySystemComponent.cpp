// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"


void UWarriorAbilitySystemComponent::OnAbilityInputPressed(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid())
		return;

	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		if (!AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag))
			continue;

		if (InInputTag.MatchesTag(WarriorGameplayTags::InputTag_Toggle) && AbilitySpec.IsActive())
		{
			CancelAbilityHandle(AbilitySpec.Handle);
			continue;
		}

		TryActivateAbility(AbilitySpec.Handle);
	}
}

void UWarriorAbilitySystemComponent::OnAbilityInputReleased(const FGameplayTag& InInputTag)
{
	if (!InInputTag.IsValid() || !InInputTag.MatchesTag(WarriorGameplayTags::InputTag_MustBeHeld))
		return;

	for (const auto& AbilitySpec : GetActivatableAbilities())
	{
		if(AbilitySpec.DynamicAbilityTags.HasTagExact(InInputTag) && AbilitySpec.IsActive())
			CancelAbilityHandle(AbilitySpec.Handle);
	}
}

void UWarriorAbilitySystemComponent::GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet> InDefaultWeaponAbilities,
	int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles)
{
	for (const auto& AbilitySet : InDefaultWeaponAbilities)
	{
		if (!AbilitySet.IsValid())
			continue;

		FGameplayAbilitySpec AbilitySpec(AbilitySet.AbilityToGrant);
		AbilitySpec.SourceObject = GetAvatarActor();
		AbilitySpec.Level = ApplyLevel;
		AbilitySpec.DynamicAbilityTags.AddTag(AbilitySet.InputTag);

		auto AbilitySpecHandle = GiveAbility(AbilitySpec); //watch for this!
		OutGrantedAbilitySpecHandles.Add(AbilitySpecHandle);
	}
}

void UWarriorAbilitySystemComponent::RemoveGrantedHeroWeaponAbilities(UPARAM(ref)TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove)
{
	for (const auto& SpecHandle : InSpecHandlesToRemove)
	{
		if (SpecHandle.IsValid())
			ClearAbility(SpecHandle);
	}

	InSpecHandlesToRemove.Empty();
}

bool UWarriorAbilitySystemComponent::TryActivateAbilityByTag(FGameplayTag Tag)
{
	check(Tag.IsValid());

	TArray<FGameplayAbilitySpec*> FoundAbilities;
	GetActivatableGameplayAbilitySpecsByAllMatchingTags(Tag.GetSingleTagContainer(), FoundAbilities);

	if (!FoundAbilities.IsEmpty())
	{
		int32 RandomIndex = FMath::RandRange(0, FoundAbilities.Num() - 1);
		auto SpecToActivte = FoundAbilities[RandomIndex];

		if (!SpecToActivte->IsActive())
			return TryActivateAbility(SpecToActivte->Handle);
	}

	return false;
}
