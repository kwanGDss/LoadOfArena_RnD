// Copyright Epic Games, Inc. All Rights Reserved.

#include "LOACharacter.h"
#include "HeadMountedDisplayFunctionLibrary.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"

//////////////////////////////////////////////////////////////////////////
// ALOACharacter

ALOACharacter::ALOACharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);

	// set our turn rates for input
	BaseTurnRate = 45.f;
	BaseLookUpRate = 45.f;

	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 360.0f, 0.0f); // ...at this rotation rate
	GetCharacterMovement()->JumpZVelocity = 600.f;
	GetCharacterMovement()->AirControl = 0.2f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 300.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = true; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named MyCharacter (to avoid direct content references in C++)

	CameraBoom->bEnableCameraLag = true;
	CameraBoom->CameraLagSpeed = 5.0f;

	GetCharacterMovement()->MaxWalkSpeed = 225.0f;

	bIsLockOnState = false;
	bIsRolling = false;

	RollStartedTime = 0.0f;
	RollDirection = FVector::ZeroVector;

	EnemyCharacter = nullptr;

	LockOnDirection = FVector::ZeroVector;
	LockOnRotation = FRotator::ZeroRotator;
	InterpToLockOn = FRotator::ZeroRotator;
}

void ALOACharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsLockOnState)
	{
		check(EnemyCharacter);
		LockOnDirection = EnemyCharacter->GetActorLocation() - GetActorLocation();
		LockOnDirection = LockOnDirection.GetSafeNormal();

		LockOnRotation = UKismetMathLibrary::MakeRotFromX(LockOnDirection);
		LockOnRotation.Pitch -= 20.0f;

		InterpToLockOn = FMath::RInterpTo(GetController()->GetControlRotation(), LockOnRotation, DeltaTime, 5.0f);

		LockOnRotation.Pitch = 0.0f;
		if (!bIsRolling)
		{
			SetActorRotation(FMath::RInterpTo(GetActorRotation(), LockOnRotation, DeltaTime, 5.0f));
		}

		GetController()->SetControlRotation(InterpToLockOn);
	}

	if (bIsRolling)
	{
		if (RollDirection == FVector::ZeroVector)
		{
			RollDirection = -GetActorForwardVector();
		}

		RollDirection = RollDirection.GetSafeNormal();
		FRotator RollRotation = UKismetMathLibrary::MakeRotFromX(RollDirection);

		SetActorLocation(FMath::VInterpTo(GetActorLocation(), RollDirection * 50.0f + GetActorLocation(), DeltaTime, 10.0f), true);
		SetActorRotation(FMath::RInterpTo(GetActorRotation(), RollRotation, DeltaTime, 5.0f));

		if (GetWorld()->TimeSince(RollStartedTime) >= 1.0f)
		{
			bIsRolling = false;
			RollDirection = FVector::ZeroVector;
		}
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void ALOACharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);
	PlayerInputComponent->BindAction("LockOn", IE_Pressed, this, &ALOACharacter::LockOn);

	PlayerInputComponent->BindAction("Sprint", IE_Pressed, this, &ALOACharacter::Run);
	PlayerInputComponent->BindAction("Sprint", IE_Released, this, &ALOACharacter::Walk);

	PlayerInputComponent->BindAction("Roll", IE_Pressed, this, &ALOACharacter::Roll);

	PlayerInputComponent->BindAxis("RollRight", this, &ALOACharacter::RollRight);
	PlayerInputComponent->BindAxis("RollForward", this, &ALOACharacter::RollForward);

	PlayerInputComponent->BindAxis("MoveForward", this, &ALOACharacter::MoveForward);
	PlayerInputComponent->BindAxis("MoveRight", this, &ALOACharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "turn" handles devices that provide an absolute delta, such as a mouse.
	// "turnrate" is for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn", this, &ALOACharacter::AddControllerYawInput);
	PlayerInputComponent->BindAxis("TurnRate", this, &ALOACharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("LookUp", this, &ALOACharacter::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("LookUpRate", this, &ALOACharacter::LookUpAtRate);

	// VR headset functionality
	PlayerInputComponent->BindAction("ResetVR", IE_Pressed, this, &ALOACharacter::OnResetVR);
}


bool ALOACharacter::IsLockOnState()
{
	return bIsLockOnState;
}

bool ALOACharacter::IsRolling()
{
	return bIsRolling;
}

void ALOACharacter::SetEnemyCharacter(TSubclassOf<ALOACharacter> EnemyCharacterClass)
{
	EnemyCharacter = Cast<ALOACharacter>(UGameplayStatics::GetActorOfClass(GetWorld(), EnemyCharacterClass));
}

ALOACharacter* ALOACharacter::GetEnemyCharacter()
{
	return EnemyCharacter;
}

void ALOACharacter::AddControllerPitchInput(float Val)
{
	if (!bIsLockOnState)
	{
		Super::AddControllerPitchInput(Val);
	}
}

void ALOACharacter::AddControllerYawInput(float Val)
{
	if (!bIsLockOnState)
	{
		Super::AddControllerYawInput(Val);
	}
}

void ALOACharacter::OnResetVR()
{
	UHeadMountedDisplayFunctionLibrary::ResetOrientationAndPosition();
}

void ALOACharacter::Run()
{
	GetCharacterMovement()->MaxWalkSpeed = 600.0f;
}

void ALOACharacter::Walk()
{
	GetCharacterMovement()->MaxWalkSpeed = 225.0f;
}

void ALOACharacter::Roll()
{
	if (!bIsRolling)
	{
		RollStartedTime = GetWorld()->GetTimeSeconds();
		bIsRolling = true;
	}
}

void ALOACharacter::LockOn()
{
	bIsLockOnState = !bIsLockOnState;

	if (bIsLockOnState)
	{
		GetCharacterMovement()->bOrientRotationToMovement = false;
	}
	else
	{
		GetCharacterMovement()->bOrientRotationToMovement = true;
	}

}

void ALOACharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void ALOACharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
}

void ALOACharacter::RollForward(float Value)
{
	if (!bIsRolling && Value != 0.0f)
	{
		if (Value > 0)
		{
			RollDirection = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X);
		}
		else
		{
			RollDirection = -FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::X);
		}
	}
}

void ALOACharacter::RollRight(float Value)
{
	if (!bIsRolling && Value != 0.0f)
	{
		if (Value > 0)
		{
			RollDirection = FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
		}
		else
		{
			RollDirection = -FRotationMatrix(FRotator(0.0f, GetControlRotation().Yaw, 0.0f)).GetUnitAxis(EAxis::Y);
		}
	}
}

void ALOACharacter::MoveForward(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !bIsRolling)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
		AddMovementInput(Direction, Value);
	}
}

void ALOACharacter::MoveRight(float Value)
{
	if ((Controller != NULL) && (Value != 0.0f) && !bIsRolling)
	{
		// find out which way is right
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get right vector 
		const FVector Direction = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);
		// add movement in that direction
		AddMovementInput(Direction, Value);
	}
}
