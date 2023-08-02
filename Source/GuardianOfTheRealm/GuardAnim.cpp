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

        // Get the lateral speed of the character from velocity
		FVector Velocity{ RealmCharacter->GetVelocity() };
		Velocity.Z = 0;
		Speed = Velocity.Size();
        if (RealmCharacter->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
        {
            bIsAccelerating = true;
        }
        else
        {
            bIsAccelerating = false;
        }

        bIsAttacking = RealmCharacter->GetIsAttacking();
    }
}

void UGuardAnim::NativeInitializeAnimation()
{
    RealmCharacter = Cast<AGuardianOfTheRealmCharacter>(TryGetPawnOwner());

}

