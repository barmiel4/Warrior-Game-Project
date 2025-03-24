// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AnimInstances/WarriorBaseAnimInstance.h"
#include "WarriorHeroLinkedAnimLayer.generated.h"


class UWarriorHeroAnimInstance;

UCLASS()
class WARRIOR_API UWarriorHeroLinkedAnimLayer : public UWarriorBaseAnimInstance
{
	GENERATED_BODY()

public:
	//functions that are both pure and tags BlueprintThreadSafe can appear in Property Acces node
	UFUNCTION(BlueprintPure, meta = (BlueprintThreadSafe)) 
	UWarriorHeroAnimInstance* GetHeroAnimInstance() const;
};
