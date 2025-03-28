// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "EnemyUIComponent.generated.h"



UCLASS()
class WARRIOR_API UEnemyUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()
	
public:
	UFUNCTION(BlueprintCallable)
	void AddWidget(UWarriorWidgetBase* Widget);

	UFUNCTION(BlueprintCallable)
	void RemoveAllWidgets();

private:
	TArray<UWarriorWidgetBase*> AddedWidgetComponents;
};
