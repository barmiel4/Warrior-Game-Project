// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WarriorHeroGameplayAbility.h"
#include "Characters/WarriorHeroCharacter.h"
#include "Controllers/WarriorHeroController.h"
#include "Components/Combat/HeroCombatComponent.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"


AWarriorHeroCharacter* UWarriorHeroGameplayAbility::GetHeroCharacterFromActorInfo()
{
	if(!CachedWarriorHeroCharacter.IsValid())
		CachedWarriorHeroCharacter = Cast<AWarriorHeroCharacter>(CurrentActorInfo->AvatarActor);
	
	return CachedWarriorHeroCharacter.IsValid() ? CachedWarriorHeroCharacter.Get() : nullptr;
}

AWarriorHeroController* UWarriorHeroGameplayAbility::GetHeroControllerFromActorInfo()
{
	if (!CachedWarriorHeroController.IsValid())
		CachedWarriorHeroController = Cast<AWarriorHeroController>(CurrentActorInfo->PlayerController);

	return CachedWarriorHeroController.IsValid() ? CachedWarriorHeroController.Get() : nullptr;
}

UHeroCombatComponent* UWarriorHeroGameplayAbility::GetHeroCombatComponentFromActorInfo()
{
	return GetHeroCharacterFromActorInfo()->GetHeroCombatComponent();
}

FGameplayEffectSpecHandle UWarriorHeroGameplayAbility::MakeHeroDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectClass, float InWeaponDamage, FGameplayTag InAttackTypeTag, int32 InUsedComboCount)
{
	check(EffectClass);

	auto AbilitySystem = GetWarriorAbilitySystemComponentFromActorInfo();
	
	//thanks to this context, other actors/abilties can know which ability was applied and who applied it
	//i.e. enemy could know that the player dealt damage with GA_DealDamage
	auto ContextHandle = AbilitySystem->MakeEffectContext();
	ContextHandle.SetAbility(this);
	ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

	auto SpecHandle = AbilitySystem->MakeOutgoingSpec(EffectClass, GetAbilityLevel(), ContextHandle);

	//this allows to associate a damage value with a specific tag
	SpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage, InWeaponDamage);

	if (InAttackTypeTag.IsValid())
	{
		SpecHandle.Data->SetSetByCallerMagnitude(InAttackTypeTag, InUsedComboCount);
	}

	return SpecHandle;
}
