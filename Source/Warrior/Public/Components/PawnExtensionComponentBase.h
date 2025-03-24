// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "PawnExtensionComponentBase.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class WARRIOR_API UPawnExtensionComponentBase : public UActorComponent
{
	GENERATED_BODY()

protected:
	template<class T>
	T* GetOwningPawn() const
	{
		//check first whether supplied class is a subclass of pawn
		static_assert(TPointerIsConvertibleFromTo<T, APawn>::Value, "Template param must derive from APawn class");
		return CastChecked<T>(GetOwner());
	}

	APawn* GetOwningPawn() const
	{
		return GetOwningPawn<APawn>();
	}

	template<class T>
	T* GetOwningContoller() const
	{
		static_assert(TPointerIsConvertibleFromTo<T, AController>::Value, "Template param must derive from AController class");
		return GetOwningPawn<APawn>()->GetController<T>();
	}


};
