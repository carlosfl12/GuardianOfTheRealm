// Fill out your copyright notice in the Description page of Project Settings.


#include "AIGuardAnim.h"
#include "Guard.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"

void UAIGuardAnim::UpdateAnimationProperties(float DeltaTime)
{
    if (Guard == nullptr)
    {
        Guard = Cast<AGuard>(TryGetPawnOwner());
    }

    if (Guard)
    {
        bIsInAir = Guard->GetCharacterMovement()->IsFalling();
        bIsMovingTowardsTarget = Guard->GetIsMovingTowardsTarget();
        bIsAttacking = Guard->GetIsAttacking();
        bIsDead = Guard->GetIsDead();
        bIsInjured = Guard->GetIsInjured();

        if (Guard->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
        {
            bIsAccelerating = true;
        }
        else
        {
            bIsAccelerating = false;
        }
    }
}

void UAIGuardAnim::NativeInitializeAnimation()
{
    Guard = Cast<AGuard>(TryGetPawnOwner());

}

void UAIGuardAnim::SetIsAttacking(bool Value)
{
    bIsAttacking = Value;
}
