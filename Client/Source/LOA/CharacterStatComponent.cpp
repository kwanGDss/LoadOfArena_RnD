// Fill out your copyright notice in the Description page of Project Settings.


#include "CharacterStatComponent.h"
#include "Components/WidgetComponent.h"

// Sets default values for this component's properties
UCharacterStatComponent::UCharacterStatComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;


	static ConstructorHelpers::FClassFinder<UUserWidget> SteminaWidgetBPClass(TEXT("/Game/UI_Stemina.UI_Stemina_C"));
	if (SteminaWidgetBPClass.Class != NULL)
	{
		SteminaWidget = SteminaWidgetBPClass.Class;
	}
	
	if (SteminaWidget != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), SteminaWidget);
		
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}


// Called when the game starts
void UCharacterStatComponent::BeginPlay()
{
	Super::BeginPlay();
	
}


// Called every frame
void UCharacterStatComponent::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	//CurrentWidget->
}

