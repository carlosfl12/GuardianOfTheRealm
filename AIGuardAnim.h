// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimInstance.h"
#include "AIGuardAnim.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANOFTHEREALM_API UAIGuardAnim : public UAnimInstance
{
	GENERATED_BODY()
	private:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Character, meta = (AllowPrivateAccess = "true"))
	class AGuard* Guard;
	/** The speed of the character */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float Speed;
	/** Whether or not the character is in the air */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsInAir;
	/** Whether or not the character is moving */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	bool bIsAccelerating;
	/** Offset yaw the frame before we stopped moving */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Movement", meta = (AllowPrivateAccess = "true"))
	float LastMovementOffsetYaw;

	/** Testing if Guard is attacking*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsMovingTowardsTarget;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category= "Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsInjured;
public:
	UFUNCTION(BlueprintCallable)
	void UpdateAnimationProperties(float DeltaTime);
	virtual void NativeInitializeAnimation() override;

	void SetIsAttacking(bool Value);
	bool GetIsAttacking() const;
};
