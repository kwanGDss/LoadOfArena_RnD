// Fill out your copyright notice in the Description page of Project Settings.


#include "LOAHUD.h"
#include "Components/WidgetComponent.h"

ALOAHUD::ALOAHUD()
{
	SteminaHUD = CreateDefaultSubobject<UWidgetComponent>(TEXT("SteminaHUD"));

	SteminaHUD->SetupAttachment(RootComponent);
	
	static ConstructorHelpers::FClassFinder<UUserWidget> HUD_Stemina(TEXT("/Game/UI_Stemina.UI_Stemina_C"));
	if(HUD_Stemina.Succeeded())
	{
		SteminaHUD->SetWidgetClass(HUD_Stemina.Class);
	}
}
