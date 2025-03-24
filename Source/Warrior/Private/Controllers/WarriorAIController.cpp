// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/WarriorAIController.h"
#include <Navigation/CrowdFollowingComponent.h>
#include <Perception/AISenseConfig_Sight.h>
#include "Perception/AIPerceptionComponent.h"
#include "GenericTeamAgentInterface.h"
#include "BehaviorTree/BlackboardComponent.h"

#include "WarriorDebugHelper.h"


AWarriorAIController::AWarriorAIController(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer.SetDefaultSubobjectClass<UCrowdFollowingComponent>("PathFollowingComponent"))
{
	checkf(Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()), TEXT("PathFollowingComponent is not valid in AI controller"));

	AISenseConfig_Sight = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("EnemySenseConfig_Sight"));
	AISenseConfig_Sight->DetectionByAffiliation.bDetectEnemies = true;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectFriendlies = false;
	AISenseConfig_Sight->DetectionByAffiliation.bDetectNeutrals = false;
	AISenseConfig_Sight->SightRadius = 5000.f;
	AISenseConfig_Sight->LoseSightRadius = 0.f; // never loose sight of player
	AISenseConfig_Sight->PeripheralVisionAngleDegrees = 360.f; // always detect/see player no matter where he is

	EnemyPerceptionComponent = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("EnemyPerceptionComponent"));
	EnemyPerceptionComponent->ConfigureSense(*AISenseConfig_Sight);
	EnemyPerceptionComponent->SetDominantSense(AISenseConfig_Sight->StaticClass());
	//when new target(i.e. the player) will be detected this delegate will fire
	EnemyPerceptionComponent->OnTargetPerceptionUpdated.AddUniqueDynamic(this, &ThisClass::OnEnemyPerceptionUpdated);

	SetGenericTeamId(FGenericTeamId(1));
}

ETeamAttitude::Type AWarriorAIController::GetTeamAttitudeTowards(const AActor& Other) const
{
	const APawn* PawnToCheck = Cast<const APawn>(&Other);

	const auto OtherTeamAgent = Cast<const IGenericTeamAgentInterface>(PawnToCheck->GetController());

	if (OtherTeamAgent && OtherTeamAgent->GetGenericTeamId() < GetGenericTeamId())
	{
		return ETeamAttitude::Hostile;
	}

	return ETeamAttitude::Friendly;
}

void AWarriorAIController::BeginPlay()
{
	Super::BeginPlay();

	if (auto CrowdComp = Cast<UCrowdFollowingComponent>(GetPathFollowingComponent()))
	{
		auto StateForDebug = bEnableCrowdAviodance ? ECrowdSimulationState::Enabled : ECrowdSimulationState::Disabled;
		CrowdComp->SetCrowdSimulationState(StateForDebug);

		CrowdComp->SetCrowdAvoidanceQuality((ECrowdAvoidanceQuality::Type)AvoidanceQuality);

		/*FString EnumString = UEnum::GetValueAsString(CrowdComp->GetCrowdAvoidanceQuality());
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Green, TEXT("Avoidance Quality = ") + EnumString);*/

		CrowdComp->SetAvoidanceGroup(1);
		CrowdComp->SetGroupsToAvoid(1);
		CrowdComp->SetCrowdCollisionQueryRange(CollisionAvoidanceRange);
	}
}

void AWarriorAIController::OnEnemyPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (Stimulus.WasSuccessfullySensed() && Actor)
	{
		if (Blackboard)
		{
			Blackboard->SetValueAsObject(FName("TargetActor"), Actor);
		}
	}
}
