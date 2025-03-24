// Fill out your copyright notice in the Description page of Project Settings.


#include "AnimInstances/WarriorBaseAnimInstance.h"
#include "WarriorFunctionLibrary.h"


bool UWarriorBaseAnimInstance::DoesOwnerHaveTag(FGameplayTag Tag) const
{
    //check macro can be used in editor functions (verify if its true)
    if (auto OwningPawn = TryGetPawnOwner())
        return UWarriorFunctionLibrary::NativeDoesActorHaveTag(OwningPawn, Tag);

    return false;
}
