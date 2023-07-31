#include "GoblinAnimInstance.h"
#include "Goblin.h"
#include "Kismet/KismetMathLibrary.h"
#include "GameFramework/Actor.h"
#include "GameFramework/CharacterMovementComponent.h"


void UGoblinAnimInstance::UpdateAnimationProperties(float DeltaTime)
{
    if (Goblin == nullptr)
    {
        Goblin = Cast<AGoblin>(TryGetPawnOwner());
    }

    if (Goblin)
    {
        bIsInAir = Goblin->GetCharacterMovement()->IsFalling();

        if (Goblin->GetCharacterMovement()->GetCurrentAcceleration().Size() > 0.f)
        {
            bIsAccelerating = true;
        }
        else
        {
            bIsAccelerating = false;
        }
    }
}

void UGoblinAnimInstance::NativeInitializeAnimation()
{
    Goblin = Cast<AGoblin>(TryGetPawnOwner());

}

void UGoblinAnimInstance::SetIsAttacking(bool Value)
{
    bIsAttacking = Value;
}

bool UGoblinAnimInstance::GetIsAttacking() const
{
    return bIsAttacking;
}

void UGoblinAnimInstance::SetIsMovingTowardsTarget(bool Value)
{
    bIsMovingTowardsTarget = Value;
}

bool UGoblinAnimInstance::GetIsMovingTowardsTarget(AActor* ThisActor, AActor *Target, float MaxDistance)
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