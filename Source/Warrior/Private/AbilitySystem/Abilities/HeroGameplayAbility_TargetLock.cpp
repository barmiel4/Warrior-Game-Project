// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/Abilities/HeroGameplayAbility_TargetLock.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Kismet/GameplayStatics.h"
#include <Widgets/WarriorWidgetBase.h>
#include "Controllers/WarriorHeroController.h"
#include "Blueprint/WidgetLayoutLibrary.h"
#include "Blueprint/WidgetTree.h"
#include "WarriorFunctionLibrary.h"
#include <Components/SizeBox.h>
#include "WarriorGameplayTags.h"
#include "Kismet/KismetMathLibrary.h"
#include "Characters/WarriorHeroCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "EnhancedInputSubsystems.h"

#include "WarriorDebugHelper.h"


#define NOT_BLOCKING_AND_ROLLING !UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetAvatarActorFromActorInfo(),WarriorGameplayTags::Player_Status_Rolling) && !UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetAvatarActorFromActorInfo(), WarriorGameplayTags::Player_Status_Blocking)


void UHeroGameplayAbility_TargetLock::ActivateAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, const FGameplayEventData* TriggerEventData)
{
	TryLockOnTarget();	
	InitTargetLockMovement();

	//add mapping context
	const auto LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
	auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

	SubSystem->AddMappingContext(TargetLockMappingContext, 3);

	Super::ActivateAbility(Handle, ActorInfo, ActivationInfo, TriggerEventData);
}

void UHeroGameplayAbility_TargetLock::EndAbility(const FGameplayAbilitySpecHandle Handle, const FGameplayAbilityActorInfo* ActorInfo, const FGameplayAbilityActivationInfo ActivationInfo, bool bReplicateEndAbility, bool bWasCancelled)
{
	Super::EndAbility(Handle, ActorInfo, ActivationInfo, bReplicateEndAbility, bWasCancelled);

	//remove input mapping context
	if (GetHeroControllerFromActorInfo())
	{
		const auto LocalPlayer = GetHeroControllerFromActorInfo()->GetLocalPlayer();
		auto SubSystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(LocalPlayer);

		SubSystem->RemoveMappingContext(TargetLockMappingContext);
	}

	CleanUpAbility();
}

void UHeroGameplayAbility_TargetLock::TargetLockUpdate(float DeltaTime)
{
	if (!CurrentLockedActor || 
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(CurrentLockedActor, WarriorGameplayTags::Shared_Status_Dead)|| 
		UWarriorFunctionLibrary::NativeDoesActorHaveTag(GetAvatarActorFromActorInfo(), WarriorGameplayTags::Shared_Status_Dead))
	{
		CancelTargeLockAbility();
		return;
	}

	SetTargetWidgetPosition();

	if (NOT_BLOCKING_AND_ROLLING)
	{
		auto TargetRot = UKismetMathLibrary::FindLookAtRotation(GetAvatarActorFromActorInfo()->GetActorLocation(), CurrentLockedActor->GetActorLocation());
		TargetRot.Roll = 0.f;
		TargetRot.Pitch -= TargetPitchOffset;

		auto HeroController = GetHeroControllerFromActorInfo();
		auto StartRot = HeroController->GetControlRotation();

		auto InterpolatedRot = FMath::RInterpTo(StartRot, TargetRot, DeltaTime, TargetUpdateRotationSpeed);
		HeroController->SetControlRotation(InterpolatedRot);
		GetAvatarActorFromActorInfo()->SetActorRotation(FRotator(0.f, InterpolatedRot.Yaw, 0.f));
	}
}

void UHeroGameplayAbility_TargetLock::SwitchTarget(const FGameplayTag& SwitchDirectionTag)
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargeLockAbility();
		return;
	}

	//huge shit...
	TArray<AActor*> ActorsOnLeft;
	TArray<AActor*> ActorsOnRight;
	AActor* NewTargetToLock = nullptr;

	const FVector PlayerLocation = GetHeroCharacterFromActorInfo()->GetActorLocation();
	const FVector PlayerToCurrentNormalized = (CurrentLockedActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

	for (AActor* AvailableActor : AvailableActorsToLock)
	{
		if (!AvailableActor || AvailableActor == CurrentLockedActor) 
			continue;

		const FVector PlayerToAvailableNormalized = (AvailableActor->GetActorLocation() - PlayerLocation).GetSafeNormal();

		const FVector CrossResult = FVector::CrossProduct(PlayerToCurrentNormalized, PlayerToAvailableNormalized);

		if (CrossResult.Z > 0.f)
		{
			ActorsOnRight.AddUnique(AvailableActor);
		}
		else
		{
			ActorsOnLeft.AddUnique(AvailableActor);
		}
	}

	if (SwitchDirectionTag == WarriorGameplayTags::Player_Event_SwitchTarget_Left)
	{
		NewTargetToLock = GetClosestActor(ActorsOnLeft);
	}
	else
	{
		NewTargetToLock = GetClosestActor(ActorsOnRight);
	}

	if (NewTargetToLock)
	{
		CurrentLockedActor = NewTargetToLock;
	}
}

void UHeroGameplayAbility_TargetLock::InitTargetLockMovement()
{
	MovComp = GetHeroCharacterFromActorInfo()->GetCharacterMovement();
	DefaultMaxWalkSpeed = MovComp->MaxWalkSpeed;

	MovComp->MaxWalkSpeed = LockedTargetMaxWalkSpeed;
}

void UHeroGameplayAbility_TargetLock::TryLockOnTarget()
{
	GetAvailableActorsToLock();

	if (AvailableActorsToLock.IsEmpty())
	{
		CancelTargeLockAbility();
		return;
	}

	CurrentLockedActor = GetClosestActor(AvailableActorsToLock);

	if (!CurrentLockedActor)
	{
		CancelTargeLockAbility();
		return;
	}

	DrawTargetLockWidget();

	SetTargetWidgetPosition();
}

void UHeroGameplayAbility_TargetLock::GetAvailableActorsToLock()
{
	AvailableActorsToLock.Empty();

	AActor* Owner = GetAvatarActorFromActorInfo();
	TArray<FHitResult> BoxTraceHits;

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		Owner,
		Owner->GetActorLocation(),
		Owner->GetActorLocation() + Owner->GetActorForwardVector() * BoxTraceDistance,
		TraceBoxSize / 2.f,
		Owner->GetActorForwardVector().ToOrientationRotator(),
		BoxTraceChannel,
		false,
		TArray<AActor*>(),
		bShowPersistentDebugShape ? EDrawDebugTrace::Persistent : EDrawDebugTrace::None,
		BoxTraceHits,
		true
	);

	for (const FHitResult& TraceHit : BoxTraceHits)
	{
		if (AActor* HitActor = TraceHit.GetActor())
		{
			if (HitActor != Owner)
				AvailableActorsToLock.AddUnique(HitActor);
		}
	}
}

void UHeroGameplayAbility_TargetLock::DrawTargetLockWidget()
{
	if (DrawnTargetWidget)
		return;

	DrawnTargetWidget = CreateWidget<UWarriorWidgetBase>(GetHeroControllerFromActorInfo(), TargetLockWidgetClass);

	DrawnTargetWidget->AddToViewport();
}

void UHeroGameplayAbility_TargetLock::SetTargetWidgetPosition()
{
	FVector2D ProjectedPosition;
	UWidgetLayoutLibrary::ProjectWorldLocationToWidgetPosition(
		GetHeroControllerFromActorInfo(),
		CurrentLockedActor->GetActorLocation(),
		ProjectedPosition,
		true
	);

	if (TargetWidgetHalfSize == FVector2D::ZeroVector)
	{
		auto Widget = DrawnTargetWidget->WidgetTree->FindWidget<USizeBox>("WarriorSizeBox");
		TargetWidgetHalfSize.X = Widget->GetWidthOverride() / 2.f;
		TargetWidgetHalfSize.Y = Widget->GetHeightOverride() / 2.f;
	}

	DrawnTargetWidget->SetPositionInViewport(ProjectedPosition - TargetWidgetHalfSize, false);
}

void UHeroGameplayAbility_TargetLock::CancelTargeLockAbility()
{
	CancelAbility(GetCurrentAbilitySpecHandle(), GetCurrentActorInfo(), GetCurrentActivationInfo(), true);
}

void UHeroGameplayAbility_TargetLock::CleanUpAbility()
{
	AvailableActorsToLock.Empty();
	CurrentLockedActor = nullptr;

	if (DrawnTargetWidget)
		DrawnTargetWidget->RemoveFromParent();

	DrawnTargetWidget = nullptr;
	TargetWidgetHalfSize = FVector2D::ZeroVector;

	MovComp->MaxWalkSpeed = DefaultMaxWalkSpeed;
}

//never called
void UHeroGameplayAbility_TargetLock::ResetTargetLockMovement()
{
	MovComp->MaxWalkSpeed = DefaultMaxWalkSpeed;
}


AActor* UHeroGameplayAbility_TargetLock::GetClosestActor(const TArray<AActor*>& Actors)
{
	float DummyDistance;
	return UGameplayStatics::FindNearestActor(GetAvatarActorFromActorInfo()->GetActorLocation(), Actors, DummyDistance);
}
