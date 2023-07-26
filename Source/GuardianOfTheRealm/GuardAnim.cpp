// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardAnim.h"
#include "GuardianOfTheRealmCharacter.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"


void UGuardAnim::UpdateAnimationProperties(float DeltaTime)
{
    if (RealmCharacter == nullptr)
    {
        RealmCharacter = Cast<AGuardianOfTheRealmCharacter>(TryGetPawnOwner());
    }

    if (RealmCharacter)
    {
        bIsInAir = RealmCharacter->GetCharacterMovement()->IsFalling();

        if (RealmCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
        {
            bIsAccelerating = true;
        }
        else
        {
            bIsAccelerating = false;
        }
    }
}

void UGuardAnim::NativeInitializeAnimation()
{
    RealmCharacter = Cast<AGuardianOfTheRealmCharacter>(TryGetPawnOwner());

}

void UGuardAnim::SetIsAttacking(bool Value)
{
    bIsAttacking = Value;
}

bool UGuardAnim::GetIsAttacking() const
{
    return bIsAttacking;
}

void UGuardAnim::SetIsMovingTowardsTarget(bool Value)
{
    bIsMovingTowardsTarget = Value;
}

bool UGuardAnim::GetIsMovingTowardsTarget(AActor* ThisActor, AActor *Target, float MaxDistance)
{
    if (Target)
    {
        float Distance = FVector::Distance(ThisActor->GetActorLocation(), Target->GetActorLocation());
        if (Distance <= MaxDistance)
        {
            return true;
        }
    }
    return false;
}
