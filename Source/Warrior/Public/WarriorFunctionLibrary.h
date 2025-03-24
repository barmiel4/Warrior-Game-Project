// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Kismet/BlueprintFunctionLibrary.h"
#include "WarriorTypes/WarriorEnumTypes.h"
#include "WarriorFunctionLibrary.generated.h"


class UWarriorAbilitySystemComponent;
class UPawnCombatComponent;


UCLASS()
class WARRIOR_API UWarriorFunctionLibrary : public UBlueprintFunctionLibrary
{
	GENERATED_BODY()
	
public:
	static UWarriorAbilitySystemComponent* NativeGetWarriorASCFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void AddGameplayTagToActorIfNone(AActor* InActor, FGameplayTag TagToAdd);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary")
	static void RemoveGameplayTagFromActor(AActor* InActor, FGameplayTag TagToRemove);

	static bool NativeDoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Does Actor Have Tag", ExpandEnumAsExecs = "OutConfirmType"))
	// DisplayName is the name that will be displayed in blueprints
	// ExpandEnumAsExecs allows to create multiple execution pins of this node in bp
	static void BP_DoesActorHaveTag(AActor* InActor, FGameplayTag TagToCheck, EWarriorConfirmType& OutConfirmType);

	static UPawnCombatComponent* NativeGetPawnCombatComponentFromActor(AActor* InActor);

	UFUNCTION(BlueprintCallable, Category = "Warrior|FunctionLibrary", meta = (DisplayName = "Get Pawn Combat Component From Actor", ExpandEnumAsExecs = "OutValidType"))
	static UPawnCombatComponent* BP_GetPawnCombatComponentFromActor(AActor* InActor, EWarriorValidType& OutValidType);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static bool IsTargetPawnHostile(APawn* QueryPawn, APawn* TargetPawn);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary", meta = (CompactNodeTitle = "Get Value At Level"))
	static float GetFloatScalableValueAtLevel(const FScalableFloat& Value, float Level);

	UFUNCTION(BlueprintPure, Category = "Warrior|FunctionLibrary")
	static int ComputeHitReactDirectionTag(AActor* Attacker, AActor* Victim);

	static bool IsValidBlock(AActor* Attacker, AActor* Target);

	static bool ApplyGameplayEffectToActor(AActor* Instigator, AActor* Target, const FGameplayEffectSpecHandle& SpecHandle);

	//for debug - to delete!
    static void DrawDebugCircleQuadrants(UWorld* World, FVector Center, FVector ForwardDirection, float Radius, float Thickness, float Duration)
    {
        if (!World) return;

        // Normalize ForwardDirection and ensure it's in the XY plane
        ForwardDirection.Z = 0;
        ForwardDirection = ForwardDirection.GetSafeNormal();

        // Compute rotated diagonal vectors for crossing lines
        FVector Diagonal1 = (ForwardDirection + FVector(-ForwardDirection.Y, ForwardDirection.X, 0)).GetSafeNormal();
        FVector Diagonal2 = (ForwardDirection + FVector(ForwardDirection.Y, -ForwardDirection.X, 0)).GetSafeNormal();

        // Draw full circle outline
        DrawDebugCircle(World, Center, Radius, 32, FColor::Black, false, Duration, 0, Thickness, Diagonal1, Diagonal2, false);

        // Draw rotated crossing lines (diagonal)
        FVector LineStart1 = Center + Diagonal1 * Radius;
        FVector LineEnd1 = Center - Diagonal1 * Radius;
        FVector LineStart2 = Center + Diagonal2 * Radius;
        FVector LineEnd2 = Center - Diagonal2 * Radius;

        DrawDebugLine(World, LineStart1, LineEnd1, FColor::Black, false, Duration, 0, Thickness); // Diagonal line 1
        DrawDebugLine(World, LineStart2, LineEnd2, FColor::Black, false, Duration, 0, Thickness); // Diagonal line 2
    }
};
