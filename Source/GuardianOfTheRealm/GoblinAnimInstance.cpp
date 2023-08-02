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
        bIsMovingTowardsTarget = Goblin->GetIsMovingTowardsTarget();
        bIsAttacking = Goblin->GetIsAttacking();

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
