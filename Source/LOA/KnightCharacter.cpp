// Fill out your copyright notice in the Description page of Project Settings.


#include "KnightCharacter.h"
#include "VikingCharacter.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

AKnightCharacter::AKnightCharacter()
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

void AKnightCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsLockOnState())
	{
		FVector LockOnDirection = EnemyCharacter->GetActorLocation() - GetActorLocation();
		LockOnDirection = LockOnDirection.GetSafeNormal();

		FRotator LockOnRotation = UKismetMathLibrary::MakeRotFromX(LockOnDirection);
		LockOnRotation.Pitch -= 20.0f;
		
		FRotator InterpToLockOn = FMath::RInterpTo(GetController()->GetControlRotation(), LockOnRotation, DeltaTime, 5.0f);

		LockOnRotation.Pitch = 0.0f;
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), LockOnRotation, DeltaTime, 5.0f));
		
		GetController()->SetControlRotation(InterpToLockOn);
	}
}

void AKnightCharacter::LockOn()
{
	Super::LockOn();

	if (IsLockOnState())
	{
		EnemyCharacter = UGameplayStatics::GetActorOfClass(GetWorld(), AVikingCharacter::StaticClass());
	}
}
