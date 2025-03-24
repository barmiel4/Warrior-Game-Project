// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTService.h"
#include "OrientTargetToActorBTService.generated.h"

/**
 * 
 */
UCLASS()
class WARRIOR_API UOrientTargetToActorBTService : public UBTService
{
	GENERATED_BODY()

public:
	UOrientTargetToActorBTService();

	UPROPERTY(EditAnywhere, Category = "Target")
	FBlackboardKeySelector InTargetActorKey;
	
	UPROPERTY(EditAnywhere, Category = "Target")
	float RotationInterpSpeed;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	virtual FString GetStaticDescription() const override;

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
};
