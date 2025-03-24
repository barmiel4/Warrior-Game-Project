// Fill out your copyright notice in the Description page of Project Settings.


#include "WarriorFunctionLibrary.h"
#include "AbilitySystemBlueprintLibrary.h"
#include "AbilitySystem/WarriorAbilitySystemComponent.h"
#include "Interfaces/PawnCombatInterface.h"
#include "GenericTeamAgentInterface.h"
#include <ScalableFloat.h>
#include "Kismet/KismetMathLibrary.h"

#include "WarriorDebugHelper.h"


UWarriorAbilitySystemComponent* UWarriorFunctionLibrary::NativeGetWarriorASCFromActor(AActor* InActor)
{
    check(InActor);

    return CastChecked<UWarriorAbilitySystemComponent>(UAbilitySystemBlueprintLibrary::GetAbilitySystemComponent(InActor));
}

void UWarriorFunctionLibrary::AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd)
{
    auto ASC = NativeGetWarriorASCFromActor(InActor); 

    if (!ASC->HasMatchingGameplayTag(TagToAdd))
    {
        ASC->AddLooseGameplayTag(TagToAdd); //adds a tag without any gameplay effect
    }
}

void UWarriorFunctionLibrary::RemoveGameplayTagFromActor(AActor* InActor, FGameplayTag TagToRemove)
{
    auto ASC = NativeGetWarriorASCFromActor(InActor);

    if (ASC->HasMatchingGameplayTag(TagToRemove))
    {
        ASC->RemoveLooseGameplayTag(TagToRemove); //adds a tag without any gameplay effect
    }
}

bool UWarriorFunctionLibrary::NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck)
{
    auto ASC = NativeGetWarriorASCFromActor(InActor);

    return ASC->HasMatchingGameplayTag(TagToCheck);
}

void UWarriorFunctionLibrary::BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType)
{
    OutConfirmType = NativeDoesActorHaveTag(InActor, TagToCheck) ? EWarriorConfirmType::Yes : EWarriorConfirmType::No;
}

UPawnCombatComponent* UWarriorFunctionLibrary::NativeGetPawnCombatComponentFromActor(AActor* InActor)
{
    check(InActor);

    if (auto FoundInterface = Cast<IPawnCombatInterface>(InActor))
        return FoundInterface->GetPawnCombatComponent();

    return nullptr;
}

UPawnCombatComponent* UWarriorFunctionLibrary::BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType)
{
    UPawnCombatComponent* pcc = NativeGetPawnCombatComponentFromActor(InActor);
    OutValidType = pcc ? EWarriorValidType::Valid : EWarriorValidType::Invalid;
    
    return pcc;
}

bool UWarriorFunctionLibrary::IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn)
{
    check(QueryPawn);
    check(TargetPawn);

    auto QueryTeamAgent = Cast<IGenericTeamAgentInterface>(QueryPawn->GetController());
    auto TargetTeamAgent = Cast<IGenericTeamAgentInterface>(TargetPawn->GetController());

    if (QueryTeamAgent && TargetTeamAgent)
        return QueryTeamAgent->GetGenericTeamId() != TargetTeamAgent->GetGenericTeamId();

    return false;
}

float UWarriorFunctionLibrary::GetFloatScalableValueAtLevel(const FScalableFloat& Value, float Level)
{
    return Value.GetValueAtLevel(Level);
}

int UWarriorFunctionLibrary::ComputeHitReactDirectionTag(AActor* Attacker, AActor* Victim)
{
    check(Attacker);
    check(Victim);

    FVector ToAttacker = (Attacker->GetActorLocation() - Victim->GetActorLocation()).GetSafeNormal2D();
    float dotf = ToAttacker.Dot(Victim->GetActorForwardVector());
    float dotr = ToAttacker.Dot(Victim->GetActorRightVector());
    int Angle = UKismetMathLibrary::DegAcos(dotf);

    /*UWorld* World = Victim->GetWorld();
    FVector Center = Victim->GetActorLocation() - FVector(0, 0, 90);
    float Radius = 150.f;
    float Duration = 0.f;
    float Thickness = 2.f;

    DrawDebugCircleQuadrants(World, Center, Victim->GetActorForwardVector(), 100.0f, 2.0f, Duration);

    DrawDebugLine(World, Center, Attacker->GetActorLocation() - FVector(0, 0, 30), FColor::White, false, Duration, 0, 4.f);*/

    if(dotr <= 0.f)
        Angle = 360 - Angle;

    Angle += 45;
    Angle %= 360;

    return Angle / 90;
}

bool UWarriorFunctionLibrary::IsValidBlock(AActor* Attacker, AActor* Target)
{
    float dot = Attacker->GetActorForwardVector().Dot(-Target->GetActorForwardVector());
    return dot >= 0.f;
}

bool UWarriorFunctionLibrary::ApplyGameplayEffectToActor(AActor* Instigator, AActor* Target, const FGameplayEffectSpecHandle& SpecHandle)
{
    auto SourceASC = NativeGetWarriorASCFromActor(Instigator);
    auto TargetASC = NativeGetWarriorASCFromActor(Target);

    auto AppliedEffectHandle = SourceASC->ApplyGameplayEffectSpecToTarget(*SpecHandle.Data, TargetASC);

    return AppliedEffectHandle.WasSuccessfullyApplied();
}

