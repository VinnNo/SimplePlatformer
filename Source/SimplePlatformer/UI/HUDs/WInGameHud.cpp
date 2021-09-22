// Fill out your copyright notice in the Description page of Project Settings.


#include "WInGameHud.h"

#include "Engine.h"

void UWInGameHud::NativeConstruct()
{
}

void UWInGameHud::UpdateSmallKeyCount(int Value)
{
	if (SmallKeyCountLabel)
	{
		FString StringValue = FString::FromInt(Value);
		FText TextValue = FText::FromString(StringValue);
		SmallKeyCountLabel->SetText(TextValue);
	}
}
