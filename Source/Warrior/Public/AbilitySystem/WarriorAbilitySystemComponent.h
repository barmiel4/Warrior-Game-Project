// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystemComponent.h"
#include <WarriorTypes/WarriorStructTypes.h>
#include "WarriorAbilitySystemComponent.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UWarriorAbilitySystemComponent : public UAbilitySystemComponent
{
	GENERATED_BODY()

public:
	void OnAbilityInputPressed(const FGameplayTag& InInputTag);
	void OnAbilityInputReleased(const FGameplayTag& InInputTag);

	UFUNCTION(BlueprintCallable, Category = "Warrior|Abilities", meta = (ApplyLevel = "1")) // this meta gives the default value in BP
	void GrantHeroWeaponAbilities(const TArray<FWarriorHeroAbilitySet> InDefaultWeaponAbilities, 
		int32 ApplyLevel, TArray<FGameplayAbilitySpecHandle>& OutGrantedAbilitySpecHandles);
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|Abilities") 
	//UPARAM(ref) tells unreal that a paramater taken by reference is not an output parameter but an acctual reference
	void RemoveGrantedHeroWeaponAbilities(UPARAM(ref) TArray<FGameplayAbilitySpecHandle>& InSpecHandlesToRemove);
	
	UFUNCTION(BlueprintCallable, Category = "Warrior|Abilities") 
	bool TryActivateAbilityByTag(FGameplayTag Tag);
};
