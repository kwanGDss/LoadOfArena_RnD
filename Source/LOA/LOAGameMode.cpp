// Copyright Epic Games, Inc. All Rights Reserved.

#include "LOAGameMode.h"
#include "LOACharacter.h"
#include "UObject/ConstructorHelpers.h"

ALOAGameMode::ALOAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/LOA.KnightCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}
}
