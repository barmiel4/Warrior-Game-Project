// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/UI/EnemyUIComponent.h"
#include "Widgets/WarriorWidgetBase.h"


void UEnemyUIComponent::AddWidget(UWarriorWidgetBase* Widget)
{
	AddedWidgetComponents.Add(Widget);
}

void UEnemyUIComponent::RemoveAllWidgets()
{
	for (auto& Widget : AddedWidgetComponents)
		Widget->RemoveFromParent();

	AddedWidgetComponents.Empty();
}
