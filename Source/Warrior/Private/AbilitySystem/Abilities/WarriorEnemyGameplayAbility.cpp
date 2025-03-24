// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/WarriorEnemyGameplayAbility.h"
#include "Characters/WarriorEnemyCharacter.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "WarriorGameplayTags.h"


AWarriorEnemyCharacter* UWarriorEnemyGameplayAbility::GetEnemyFromActorInfo()
{
    if (!CachedEnemyCharacter.IsValid())
    {
        CachedEnemyCharacter = Cast<AWarriorEnemyCharacter>(CurrentActorInfo->AvatarActor);
    }

    return CachedEnemyCharacter.IsValid() ? CachedEnemyCharacter.Get() : nullptr;
}

UEnemyCombatComponent* UWarriorEnemyGameplayAbility::GetEnemyCombatComponentFromActorInfo()
{
    return GetEnemyFromActorInfo()->EnemyCombatComponent;
}

FGameplayEffectSpecHandle UWarriorEnemyGameplayAbility::MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectToApply, const FScalableFloat& DamageToApply)
{
    check(EffectToApply);

    auto AbilitySystem = GetWarriorAbilitySystemComponentFromActorInfo();

    //thanks to this context, other actors/abilties can know which ability was applied and who applied it
    //i.e. enemy could know that the player dealt damage with GA_DealDamage
    auto ContextHandle = AbilitySystem->MakeEffectContext();
    ContextHandle.SetAbility(this);
    ContextHandle.AddSourceObject(GetAvatarActorFromActorInfo());

    auto SpecHandle = AbilitySystem->MakeOutgoingSpec(EffectToApply, GetAbilityLevel(), ContextHandle);

    //this allows to associate a damage value with a specific tag
    SpecHandle.Data->SetSetByCallerMagnitude(WarriorGameplayTags::Shared_SetByCaller_BaseDamage,
        DamageToApply.GetValueAtLevel(GetAbilityLevel()));

    return SpecHandle;
}
