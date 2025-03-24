// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "WarriorAIController.generated.h"


class UAISenseConfig_Sight;


UCLASS()
class WARRIOR_API AWarriorAIController : public AAIController
{
	GENERATED_BODY()

public:
	AWarriorAIController(const FObjectInitializer& ObjectInitializer);

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance")
	bool bEnableCrowdAviodance = false;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance", meta = (EditCondition = "bEnableCrowdAviodance", UIMin = "0", UIMax = "4"))
	//UIMin and max produce a slider in editor
	int32 AvoidanceQuality = 4;

	UPROPERTY(EditDefaultsOnly, Category = "Detour Crowd Avoidance", meta = (EditCondition = "bEnableCrowdAviodance"))
	float CollisionAvoidanceRange = 600.f;


	virtual ETeamAttitude::Type GetTeamAttitudeTowards(const AActor& Other) const override;
	
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAIPerceptionComponent* EnemyPerceptionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UAISenseConfig_Sight* AISenseConfig_Sight;

	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);
};
