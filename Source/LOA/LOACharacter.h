// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "LOACharacter.generated.h"

UCLASS(config=Game)
class ALOACharacter : public ACharacter
{
	GENERATED_BODY()

	/** Camera boom positioning the camera behind the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	class USpringArmComponent* CameraBoom;

	/** Follow camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = true))
	class UCameraComponent* FollowCamera;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = LockOn, Meta = (AllowPrivateAccess = true))
	bool bIsLockOnState;

	UPROPERTY(EditInstanceOnly, BlueprintReadOnly, Category = LockOn, Meta = (AllowPrivateAccess = true))
	class ALOACharacter* EnemyCharacter;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = Movement, Meta = (AllowPrivateAccess = true))
	bool bIsRolling;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = Movement, Meta = (AllowPrivateAccess = true))
	float RollStartedTime;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Category = Movement, Meta = (AllowPrivateAccess = true))
	float RollDelay;

	UPROPERTY()
	class TSubclassOf<UUserWidget> StaminaWidget;

	UPROPERTY()
    UUserWidget* CurrentWidget;

	UPROPERTY()
	bool bIsShift;
	
	UPROPERTY()
	bool bIsRunning;

	UPROPERTY()
	bool bIsGuard;
	
	UPROPERTY()
	float Stamina;
	
	UPROPERTY()
	class UProgressBar* StaminaProgressBar;
	
public:
	ALOACharacter();

	virtual void Tick(float DeltaTime) override;
	
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseTurnRate;

	/** Base look up/down rate, in deg/sec. Other scaling may affect final rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float BaseLookUpRate;

	bool IsLockOnState();

	bool IsRolling();

	void SetEnemyCharacter(TSubclassOf<ALOACharacter> EnemyCharacter1);
	ALOACharacter* GetEnemyCharacter();
	
	virtual void AddControllerPitchInput(float Val) override;
	virtual void AddControllerYawInput(float Val) override;

protected:

	/** Resets HMD orientation in VR. */
	void OnResetVR();

	/** Called for forwards/backward input */
	void MoveForward(float Value);

	/** Called for side to side input */
	void MoveRight(float Value);

	/** 
	 * Called via input to turn at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate. 
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void RollForward(float Value);
	void RollRight(float Value);
	
	void Run();

	void Walk();

	void Roll();

	void LeftAttack();
	void RightAttack();

	void Guard();
	
	virtual void LockOn();

	// APawn interface
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;
	// End of APawn interface
	
protected:
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = Movement, Meta = (AllowPrivateAccess = true))
	FVector RollDirection;

	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = Movement, Meta = (AllowPrivateAccess = true))
	FVector LockOnDirection;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = Movement, Meta = (AllowPrivateAccess = true))
	FRotator LockOnRotation;
	
	UPROPERTY(VisibleInstanceOnly, BlueprintReadOnly, Transient, Category = Movement, Meta = (AllowPrivateAccess = true))
	FRotator InterpToLockOn;

public:
	/** Returns CameraBoom subobject **/
	FORCEINLINE class USpringArmComponent* GetCameraBoom() const { return CameraBoom; }
	/** Returns FollowCamera subobject **/
	FORCEINLINE class UCameraComponent* GetFollowCamera() const { return FollowCamera; }
};

