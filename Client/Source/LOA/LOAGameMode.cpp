// Copyright Epic Games, Inc. All Rights Reserved.

#include "LOAGameMode.h"
#include "LOACharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "Components/WidgetComponent.h"

ALOAGameMode::ALOAGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnBPClass(TEXT("/Script/LOA.KnightCharacter"));
	if (PlayerPawnBPClass.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnBPClass.Class;
	}

	/*static ConstructorHelpers::FClassFinder<UUserWidget> SteminaWidgetBPClass(TEXT("/Game/UI_Stemina.UI_Stemina_C"));
	if (SteminaWidgetBPClass.Class != NULL)
	{
		SteminaWidget = SteminaWidgetBPClass.Class;
	}*/

	//if (SteminaWidget != nullptr)
	//{
	//	CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), SteminaWidget);
	//	
	//	if (CurrentWidget != nullptr)
	//	{
	//		CurrentWidget->AddToViewport();
	//	}
	//}
}
