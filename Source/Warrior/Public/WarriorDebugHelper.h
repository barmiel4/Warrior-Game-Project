#pragma once


namespace Debug
{
	static void Print(const FString& Msg, const FColor& Color = FColor::Orange, int32 InKey = -1)
	{
		GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, Msg);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *Msg);
	}

	static void Print(const FString& Msg, float Value, const FColor& Color = FColor::Green, int32 InKey = -1)
	{
		FString out = Msg + TEXT(": ") + FString::SanitizeFloat(Value);

		GEngine->AddOnScreenDebugMessage(InKey, 7.f, Color, out);

		UE_LOG(LogTemp, Warning, TEXT("%s"), *out);
	}
}