// Copyright Epic Games, Inc. All Rights Reserved.

#include "Project3TutorialGameMode.h"
#include "Project3TutorialCharacter.h"
#include "UObject/ConstructorHelpers.h"

AProject3TutorialGameMode::AProject3TutorialGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Game/ThirdPerson/Blueprints/BP_ThirdPersonCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
