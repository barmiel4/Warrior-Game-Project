// Fill out your copyright notice in the Description page of Project Settings.


#include "DataAssets/Input/DataAsset_InputConfig.h"

UInputAction* UDataAsset_InputConfig::FindNativeInputActionByTag(const FGameplayTag& InInputTag) const
{
	for (const auto& InputActionConfig : NativeInputActions)
	{
		if (InputActionConfig.InputAction && InputActionConfig.InputTag == InInputTag)
			return InputActionConfig.InputAction;
	}

	return nullptr;
}
