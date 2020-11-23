// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightCharacter.h"
#include "VikingCharacter.h"
#include "Components/PoseableMeshComponent.h"
#include "Kismet/GameplayStatics.h"

AKnightCharacter::AKnightCharacter()
{
	PrimaryActorTick.bCanEverTick = true;

	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -96.0f), FRotator(0.0f, -90.0f, 0.0f));

	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/Mesh/Knight_Focus_Run_2020_09_05_V1.Knight_Focus_Run_2020_09_05_V1"));

	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);

	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_Mannequin(TEXT("/Game/Anim/AnimBP/Knight_Focus_2020_AnimBP1.Knight_Focus_2020_AnimBP1_C"));

	if (Anim_Mannequin.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Mannequin.Class);
	}
	
	SetEnemyCharacter(AVikingCharacter::StaticClass());
}

void AKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AKnightCharacter::LockOn()
{
	Super::LockOn();
	
}
