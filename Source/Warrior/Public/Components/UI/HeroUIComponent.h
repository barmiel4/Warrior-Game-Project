// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/UI/PawnUIComponent.h"
#include "HeroUIComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnCurrentWeaponChanged, TSoftObjectPtr<UTexture2D>, WeaponIcon);


UCLASS()
class WARRIOR_API UHeroUIComponent : public UPawnUIComponent
{
	GENERATED_BODY()

public:
	UPROPERTY(BlueprintAssignable)
	FOnPercentChanged OnCurrentRageChanged;

	UPROPERTY(BlueprintAssignable, BlueprintCallable)
	FOnCurrentWeaponChanged OnCurrentWeaponChanged;
};
