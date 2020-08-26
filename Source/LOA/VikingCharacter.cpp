// Fill out your copyright notice in the Description page of Project Settings.


#include "VikingCharacter.h"
#include "Kismet/GameplayStatics.h"

AVikingCharacter::AVikingCharacter()
{
	PrimaryActorTick.bCanEverTick = true;
	
	GetMesh()->SetRelativeLocationAndRotation(FVector(0.0f, 0.0f, -93.0f), FRotator(0.0f, -90.0f, 0.0f));
	
	static ConstructorHelpers::FObjectFinder<USkeletalMesh> SK_Mannequin(TEXT("/Game/Mannequin/Character/Mesh/SK_Mannequin.SK_Mannequin"));

	if (SK_Mannequin.Succeeded())
	{
		GetMesh()->SetSkeletalMesh(SK_Mannequin.Object);
	}

	GetMesh()->SetAnimationMode(EAnimationMode::AnimationBlueprint);
	
	static ConstructorHelpers::FClassFinder<UAnimInstance> Anim_Mannequin(TEXT("/Game/Mannequin/Animations/ThirdPerson_AnimBP.ThirdPerson_AnimBP_C"));

	if (Anim_Mannequin.Succeeded())
	{
		GetMesh()->SetAnimInstanceClass(Anim_Mannequin.Class);
	}
}