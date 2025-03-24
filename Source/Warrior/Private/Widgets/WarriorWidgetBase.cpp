// Fill out your copyright notice in the Description page of Project Settings.


#include "Widgets/WarriorWidgetBase.h"
#include "Interfaces/PawnUIInterface.h"
#include "Components/UI/HeroUIComponent.h"
#include "Components/UI/EnemyUIComponent.h"

void UWarriorWidgetBase::NativeOnInitialized()
{
	Super::NativeOnInitialized();

	if (auto PawnUIInterface = Cast<IPawnUIInterface>(GetOwningPlayerPawn()))
	{
		if (auto HeroUIComponent = PawnUIInterface->GetHeroUIComponent())
		{
			BP_OnHeroUIComponentInit(HeroUIComponent);
		}
	}
}

void UWarriorWidgetBase::InitEnemyCharacterWidget(AActor* OwningEnemyActor)
{
	if (auto PawnUIInterface = Cast<IPawnUIInterface>(OwningEnemyActor))
	{
		auto EnemyUIComponent = Cast<UEnemyUIComponent>(PawnUIInterface->GetPawnUIComponent());

		checkf(EnemyUIComponent, TEXT("Enemy %s has not UI Component"), *OwningEnemyActor->GetActorNameOrLabel());

		BP_OnEnemyUIComponentInit(EnemyUIComponent);
	}
}
