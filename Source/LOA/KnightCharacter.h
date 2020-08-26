// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "LOACharacter.h"
#include "KnightCharacter.generated.h"

/**
 * 
 */
UCLASS()
class LOA_API AKnightCharacter : public ALOACharacter
{
	GENERATED_BODY()
	
public:
	AKnightCharacter();

	virtual void Tick(float DeltaTime) override;

protected:
	virtual void LockOn() override;

private:
	UPROPERTY(EditAnywhere, Category = LockOn, Meta = (AllowPrivateAccess = true))
	class AActor* EnemyCharacter;
};
