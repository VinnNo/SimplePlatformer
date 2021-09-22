// Copyright Epic Games, Inc. All Rights Reserved.

#include "SimplePlatformerGameMode.h"
#include "SimplePlatformerCharacter.h"
#include "UObject/ConstructorHelpers.h"

ASimplePlatformerGameMode::ASimplePlatformerGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
