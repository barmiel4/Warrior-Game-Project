// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/AbilityTasks/AbilityTask_ExecuteTastOnTick.h"

UAbilityTask_ExecuteTastOnTick::UAbilityTask_ExecuteTastOnTick()
{
	bTickingTask = true;
}

UAbilityTask_ExecuteTastOnTick* UAbilityTask_ExecuteTastOnTick::ExectuteTaskOnTick(UGameplayAbility* OwningAbility)
{
	auto Node = NewAbilityTask<UAbilityTask_ExecuteTastOnTick>(OwningAbility);

	return Node;
}

void UAbilityTask_ExecuteTastOnTick::TickTask(float DeltaTime)
{
	Super::TickTask(DeltaTime);

	if (ShouldBroadcastAbilityTaskDelegates())
		OnAbilityTaskTick.Broadcast(DeltaTime);
	else
		EndTask();
}
