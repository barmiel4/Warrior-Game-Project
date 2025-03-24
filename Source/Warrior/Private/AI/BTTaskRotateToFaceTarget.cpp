// Fill out your copyright notice in the Description page of Project Settings.


#include "AI/BTTaskRotateToFaceTarget.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "AIController.h"
#include "Kismet/KismetMathLibrary.h"


UBTTaskRotateToFaceTarget::UBTTaskRotateToFaceTarget()
{
	NodeName = TEXT("Rotate To Face Target Actor");
	AnglePrecision = 10.f;
	InterpSpeed = 5.f;

	bNotifyTick = true;
	bNotifyTaskFinished = true;

	INIT_TASK_NODE_NOTIFY_FLAGS();

	TargetToFace.AddObjectFilter(this, GET_MEMBER_NAME_CHECKED(ThisClass, TargetToFace), AActor::StaticClass());
}

void UBTTaskRotateToFaceTarget::InitializeFromAsset(UBehaviorTree& Asset)
{
	Super::InitializeFromAsset(Asset);

	if (auto Blackboard = GetBlackboardAsset())
	{
		TargetToFace.ResolveSelectedKey(*Blackboard);
	}
}

uint16 UBTTaskRotateToFaceTarget::GetInstanceMemorySize() const
{
	return sizeof(FRotateToFaceTargetTaskMemory);
}

FString UBTTaskRotateToFaceTarget::GetStaticDescription() const
{
	const FString KeyDescription = TargetToFace.SelectedKeyName.ToString();

	return "Rotate To Face " + KeyDescription + " Key until the " + FString::SanitizeFloat(AnglePrecision) + " degrees threshold is reached";
}

EBTNodeResult::Type UBTTaskRotateToFaceTarget::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	UObject* TargetAsObject = OwnerComp.GetBlackboardComponent()->GetValueAsObject(TargetToFace.SelectedKeyName);
	AActor* TargetActor = Cast<AActor>(TargetAsObject);

	APawn* OwningPawn = OwnerComp.GetAIOwner()->GetPawn();

	auto TaskMemoryHandle = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);
	check(TaskMemoryHandle);

	TaskMemoryHandle->OwningPawn = OwningPawn;
	TaskMemoryHandle->TargetActor = TargetActor;

	if (!TaskMemoryHandle->IsValid())
		return EBTNodeResult::Failed;

	if (HasReachedAngle(TaskMemoryHandle))
	{
		TaskMemoryHandle->Reset();
		return EBTNodeResult::Succeeded;
	}

	return EBTNodeResult::InProgress;
}

void UBTTaskRotateToFaceTarget::TickTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	auto TaskMemoryHandle = CastInstanceNodeMemory<FRotateToFaceTargetTaskMemory>(NodeMemory);

	if (!TaskMemoryHandle->IsValid())
		FinishLatentTask(OwnerComp, EBTNodeResult::Failed);

	if (HasReachedAngle(TaskMemoryHandle))
	{
		TaskMemoryHandle->Reset();
		FinishLatentTask(OwnerComp, EBTNodeResult::Succeeded);
	}
	else
	{
		auto Enemy = TaskMemoryHandle->OwningPawn;
		auto Target = TaskMemoryHandle->TargetActor;

		FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), Target->GetActorLocation());
		Enemy->SetActorRotation(FMath::RInterpTo(Enemy->GetActorRotation(), LookAt, DeltaSeconds, InterpSpeed));
	}
}

bool UBTTaskRotateToFaceTarget::HasReachedAngle(const FRotateToFaceTargetTaskMemory* MemTask) const
{
	auto Enemy = MemTask->OwningPawn;
	auto Target = MemTask->TargetActor;

	FRotator LookAt = UKismetMathLibrary::FindLookAtRotation(Enemy->GetActorLocation(), Target->GetActorLocation());
	return FMath::Abs((LookAt - Enemy->GetActorRotation()).Yaw) <= AnglePrecision;
}