// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin.h"
#include "GuardianOfTheRealmCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GoblinAnimInstance.h"

// Sets default values
AGoblin::AGoblin() :
	// Combat
	bIsAttacking(false),
	Speed(0),
	CurrentTarget(nullptr),
	bIsMovingTowardsTarget(false),
	CombatState(ECombatState::ECS_Idle)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/**
 * TODO: StateMachine
 * Fix enemy cant move while standard attack
 * 
*/
// Called when the game starts or when spawned
void AGoblin::BeginPlay()
{
	Super::BeginPlay();
	RealmCharacter = GetRealmCharacter();

	USkeletalMeshComponent* SkeletalMeshComponent = GetMesh();

	if (SkeletalMeshComponent && SkeletalMeshComponent->AnimClass)
	{
		GoblinAnimInstance = Cast<UAnimInstance>(SkeletalMeshComponent->GetAnimInstance());
	}
}


// Called to bind functionality to input
void AGoblin::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}


AGuardianOfTheRealmCharacter* AGoblin::GetRealmCharacter() const
{
	return Cast<AGuardianOfTheRealmCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

AGuardianOfTheRealmCharacter *AGoblin::GetClosestTarget(AActor *Caller) const
{
	/**
	 * TODO: Make the goblin target the closest target
	*/
	TArray<AActor*> FoundActors;
	UGameplayStatics::GetAllActorsOfClass(Caller->GetWorld(), AGuardianOfTheRealmCharacter::StaticClass(), FoundActors);

	AGuardianOfTheRealmCharacter* ClosestTarget = nullptr;

	
	for (AActor* FoundActor : FoundActors)
	{
		// Cast found actor into AGuardianOfTheRealmCharacter
		float ClosestDistance = MAX_FLT;
		if (AGuardianOfTheRealmCharacter* GuardianCharacter = Cast<AGuardianOfTheRealmCharacter>(FoundActor))
		{
			float Distance = FVector::Distance(FoundActor->GetActorLocation(), Caller->GetActorLocation());
			UE_LOG(LogTemp, Display, TEXT("Distance: %f"), Distance);
			if (Distance <= ClosestDistance)
			{
				ClosestDistance = Distance;
				ClosestTarget = GuardianCharacter;
				UE_LOG(LogTemp, Display, TEXT("Entro entro entro: %s"), *ClosestTarget->GetActorNameOrLabel());
			}

		}
	}
	return ClosestTarget;
}

void AGoblin::MoveToPlayer()
{
	if (CurrentTarget)
	{	
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(CurrentTarget->GetActorLocation() - GetActorLocation()).Rotator();
		SetActorRotation(LookAtRotation);

		FVector MoveDirection = CurrentTarget->GetActorLocation() - GetActorLocation();
		MoveDirection.Normalize();

		float Distance = FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation());
		if (Distance > StoppingDistance && CombatState == ECombatState::ECS_Moving)
		{
			// Continue moving towards target
			bIsMovingTowardsTarget = true;
			AddMovementInput(MoveDirection, .3f);
		} 
		else
		{
			// No need to continue moving towards target
			bIsMovingTowardsTarget = false;
			AddMovementInput(MoveDirection, 0.f);
		}
	}
}
bool AGoblin::GetIsMovingTowardsTarget(AActor *Target, float MaxDistance)
{
	if (Target)
    {
        float Distance = FVector::Distance(GetActorLocation(), Target->GetActorLocation());
        if (Distance <= MaxDistance)
        {
			CombatState = ECombatState::ECS_Moving;
            return true;
        }
    }
	CombatState = ECombatState::ECS_Idle;
    return false;
}
void AGoblin::AttackTarget()
{
	/**
	 * Calls Anim
	 * Sets attack in true
	 * Needs to be refactored later
	*/
	UGoblinAnimInstance* GoblinAnim = Cast<UGoblinAnimInstance>(GoblinAnimInstance);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	Speed = GetVelocity().Size();
	if (GoblinAnim && AttackMontage && AnimInstance && Speed > 0.f)
	{
		// Mixes movement with the attack montage
		// Needs to attack once every X seconds
		GoblinAnim->SetIsAttacking(true);
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("Attack"));
	}
	else if (Speed <= 0.f)
	{
		CombatState = ECombatState::ECS_Attacking;
		bIsAttacking = true;
		GetWorldTimerManager().SetTimer(AttackTimerHandle, this, &AGoblin::EndAttack, AttackDuration, false);

	}

}

void AGoblin::EndAttack()
{
	if (CurrentTarget)
	{
		float Distance = FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation());
		if (Distance > AttackRange && Distance < DistanceDetection)
		{
			CombatState = ECombatState::ECS_Moving;
		} 
		else
		{
			CombatState = ECombatState::ECS_Idle;
		}
	}
}

void AGoblin::Idle()
{
	bIsAttacking = false;
	bIsMovingTowardsTarget = false;
	CurrentTarget = GetClosestTarget(this);
}

void AGoblin::GoblinLogic()
{
	CurrentTarget = GetClosestTarget(this);
	if (CurrentTarget)
	{
		float DistanceToPlayer = FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation());

		if (DistanceToPlayer <= AttackRange)
		{
			AttackTarget();
		}
		else if (GetIsMovingTowardsTarget(CurrentTarget, DistanceDetection))
		{
			MoveToPlayer();
		}
	}
	else
	{
		Idle();
	}
	UE_LOG(LogTemp, Display, TEXT("Target: %s"), *CurrentTarget->GetActorNameOrLabel());
}

// Called every frame
void AGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GoblinLogic();
}
