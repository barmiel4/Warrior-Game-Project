// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AbilitySystem/Abilities/WarriorGameplayAbility.h"
#include "WarriorEnemyGameplayAbility.generated.h"


class AWarriorEnemyCharacter;
class UEnemyCombatComponent;
class UEnemyCombatComponent;

UCLASS()
class WARRIOR_API UWarriorEnemyGameplayAbility : public UWarriorGameplayAbility
{
	GENERATED_BODY()
public:
	UFUNCTION(BlueprintPure)
	AWarriorEnemyCharacter* GetEnemyFromActorInfo();
	
	UFUNCTION(BlueprintPure)
	UEnemyCombatComponent* GetEnemyCombatComponentFromActorInfo();

	UFUNCTION(BlueprintPure)
	FGameplayEffectSpecHandle MakeEnemyDamageEffectSpecHandle(TSubclassOf<UGameplayEffect> EffectToApply, const FScalableFloat& DamageToApply);

private:
	TWeakObjectPtr<AWarriorEnemyCharacter> CachedEnemyCharacter;
	TWeakObjectPtr<UEnemyCombatComponent> CachedCombatComponent;
};
