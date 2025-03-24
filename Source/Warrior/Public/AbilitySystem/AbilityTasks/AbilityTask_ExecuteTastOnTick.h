// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Abilities/Tasks/AbilityTask.h"
#include "AbilityTask_ExecuteTastOnTick.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnAbilityTaskTick, float, DeltaTime);

UCLASS()
class WARRIOR_API UAbilityTask_ExecuteTastOnTick : public UAbilityTask
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnAbilityTaskTick OnAbilityTaskTick;

	UAbilityTask_ExecuteTastOnTick();

	UFUNCTION(BlueprintCallable, meta = (HidePin = "OwningAbility", DefaultToSelf = "OwningAbility", BlueprintInternalUseOnly = "true"))
	static UAbilityTask_ExecuteTastOnTick* ExectuteTaskOnTick(UGameplayAbility* OwningAbility);

	virtual void TickTask(float DeltaTime) override;
};
