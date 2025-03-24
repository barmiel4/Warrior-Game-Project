// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "WarriorWidgetBase.generated.h"


UCLASS()
class WARRIOR_API UWarriorWidgetBase : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void NativeOnInitialized() override;

	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Hero UI Component Init"))
	void BP_OnHeroUIComponentInit(UHeroUIComponent* HeroUIComponent);
	
	UFUNCTION(BlueprintImplementableEvent, meta = (DisplayName = "On Enemy UI Component Init"))
	void BP_OnEnemyUIComponentInit(UEnemyUIComponent* EnemyUIComponent);

public:
	UFUNCTION(BlueprintCallable)
	void InitEnemyCharacterWidget(AActor* OwningEnemyActor);
};
