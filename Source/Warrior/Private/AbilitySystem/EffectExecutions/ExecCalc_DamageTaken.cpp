// Fill out your copyright notice in the Description page of Project Settings.


#include "AbilitySystem/EffectExecutions/ExecCalc_DamageTaken.h"
#include "AbilitySystem/WarriorAttributeSet.h"
#include "WarriorGameplayTags.h"
#include "WarriorDebugHelper.h"


struct FWarriorDamageCapture
{
	DECLARE_ATTRIBUTE_CAPTUREDEF(AttackPower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DefensePower)
	DECLARE_ATTRIBUTE_CAPTUREDEF(DamageTaken)

	FWarriorDamageCapture()
	{
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, AttackPower, Source, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DefensePower, Target, false);
		DEFINE_ATTRIBUTE_CAPTUREDEF(UWarriorAttributeSet, DamageTaken, Target, false);
	}
};

static FWarriorDamageCapture& GetWarriorDamageCapture()
{
	static FWarriorDamageCapture st;

	return st;
}

UExecCalc_DamageTaken::UExecCalc_DamageTaken()
{
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().AttackPowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DefensePowerDef);
	RelevantAttributesToCapture.Add(GetWarriorDamageCapture().DamageTakenDef);
}

void UExecCalc_DamageTaken::Execute_Implementation(const FGameplayEffectCustomExecutionParameters& ExecutionParams, FGameplayEffectCustomExecutionOutput& OutExecutionOutput) const
{
	const auto& EffectSpec = ExecutionParams.GetOwningSpec();

	FAggregatorEvaluateParameters EvalParams;
	EvalParams.SourceTags = EffectSpec.CapturedSourceTags.GetAggregatedTags();
	EvalParams.TargetTags = EffectSpec.CapturedTargetTags.GetAggregatedTags();

	float SourceAttackPower;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().AttackPowerDef, EvalParams, SourceAttackPower);
	//Debug::Print("Source Attack Power", SourceAttackPower);

	auto& TagMap = EffectSpec.SetByCallerTagMagnitudes;
	float BaseDamage =					TagMap[WarriorGameplayTags::Shared_SetByCaller_BaseDamage];
	int32 UsedLightAttackComboCount =	TagMap.FindRef(WarriorGameplayTags::Player_SetByCaller_AttackType_Light);
	int32 UsedHeavyAttackComboCount =	TagMap.FindRef(WarriorGameplayTags::Player_SetByCaller_AttackType_Heavy);

	/*Debug::Print("Base Damage", BaseDamage);
	Debug::Print("UsedLightAttackComboCount", UsedLightAttackComboCount);
	Debug::Print("UsedHeavyAttackComboCount", UsedHeavyAttackComboCount);*/

	float TargetDefensePower;
	ExecutionParams.AttemptCalculateCapturedAttributeMagnitude(GetWarriorDamageCapture().DefensePowerDef, EvalParams, TargetDefensePower);
	//Debug::Print("Target Defense Power", TargetDefensePower);


	if (UsedLightAttackComboCount)
	{
		const float DamageIncreasePercent = (UsedLightAttackComboCount - 1) * 0.05f + 1.f;

		BaseDamage *= DamageIncreasePercent;
	}

	if (UsedHeavyAttackComboCount)
	{
		const float DamagaIncreasePercent = UsedHeavyAttackComboCount * 0.25f + 1.f;

		BaseDamage *= DamagaIncreasePercent;
	}

	const float FinalDamage = BaseDamage * SourceAttackPower / TargetDefensePower;
	Debug::Print("Final Damage", FinalDamage, FColor::Orange);

	if (FinalDamage > 0.f)
	{
		OutExecutionOutput.AddOutputModifier(
			FGameplayModifierEvaluatedData(
				GetWarriorDamageCapture().DamageTakenProperty,
				EGameplayModOp::Override,
				FinalDamage
			)
		);
	}
}
