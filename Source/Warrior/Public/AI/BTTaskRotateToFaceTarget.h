// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BehaviorTree/BTTaskNode.h"
#include "BTTaskRotateToFaceTarget.generated.h"

//this struct is needed if we want to preserve variable values between this task's executions
//blueprint versions of BTTasks do this by default
struct FRotateToFaceTargetTaskMemory
{
	TWeakObjectPtr<APawn> OwningPawn;
	TWeakObjectPtr<AActor> TargetActor;

	bool IsValid() const
	{
		return OwningPawn.IsValid() && TargetActor.IsValid();
	}

	void Reset()
	{
		OwningPawn.Reset();
		TargetActor.Reset();
	}
};

UCLASS()
class WARRIOR_API UBTTaskRotateToFaceTarget : public UBTTaskNode
{
	GENERATED_BODY()
	
public:
	UBTTaskRotateToFaceTarget();

	UPROPERTY(EditAnywhere, Category = "Face Target")
	float AnglePrecision;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	float InterpSpeed;

	UPROPERTY(EditAnywhere, Category = "Face Target")
	FBlackboardKeySelector TargetToFace;

	bool HasReachedAngle(const FRotateToFaceTargetTaskMemory* MemTask) const;

	virtual void InitializeFromAsset(UBehaviorTree& Asset) override;

	//needed to preserve variables values between this task's executions
	virtual uint16 GetInstanceMemorySize() const override;

	virtual FString GetStaticDescription() const override;

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;

	virtual void TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;

};
