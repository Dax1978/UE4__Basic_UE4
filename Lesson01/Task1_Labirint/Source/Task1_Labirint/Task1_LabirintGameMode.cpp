// Copyright Epic Games, Inc. All Rights Reserved.

#include "Task1_LabirintGameMode.h"
#include "Task1_LabirintCharacter.h"
#include "UObject/ConstructorHelpers.h"

ATask1_LabirintGameMode::ATask1_LabirintGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPersonCPP/Blueprints/ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
