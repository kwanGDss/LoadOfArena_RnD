// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "LOAHUD.generated.h"

/**
 * 
 */
UCLASS()
class LOA_API ALOAHUD : public AHUD
{
	GENERATED_BODY()

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = HUD, Meta = (AllowPrivateAccess = true))
	class UWidgetComponent* SteminaHUD; 
	
public:
	ALOAHUD();
};
