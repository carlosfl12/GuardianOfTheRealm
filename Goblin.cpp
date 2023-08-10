// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin.h"
#include "GuardianOfTheRealmCharacter.h"
#include "Guard.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GoblinAnimInstance.h"


// Sets default values
AGoblin::AGoblin() :
	// Combat
	MaxHealth(100.f),
	CurrentHealth(100.f),
	bIsAttacking(false),
	Damage(30.f),
	Speed(0),
	CurrentTarget(nullptr),
	bIsMovingTowardsTarget(false),
	CombatState(ECombatState::ECS_Idle),
	bIsDead(false)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

/**
 * TODO: Running State, Refactor, Detect Guard NPC
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

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	Weapon = AttachedActors[0];
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

AActor *AGoblin::GetClosestTarget() const
{
	TArray<AActor*> FoundActors; 
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AActor::StaticClass(), FoundActors);

    float ClosestDistanceSquared = DistanceDetection * DistanceDetection;
    AActor* ClosestTarget = nullptr;

    for (AActor* FoundActor : FoundActors)
    {
		AGoblin* Goblin = Cast<AGoblin>(FoundActor);
        if (!Goblin)
        {
			if (AGuardianOfTheRealmCharacter* GuardianCharacter = Cast<AGuardianOfTheRealmCharacter>(FoundActor))
			{
				if (GuardianCharacter->GetIsDead())
				{
					continue;
				}
				float DistanceSquared = FVector::DistSquared(FoundActor->GetActorLocation(), GetActorLocation());
				if (DistanceSquared <= ClosestDistanceSquared)
				{
					ClosestDistanceSquared = DistanceSquared;
					ClosestTarget = GuardianCharacter;
				}
			}	

			else if (AGuard* Guard = Cast<AGuard>(FoundActor))
			{
				if (Guard->GetIsDead())
				{
					continue;
				}
				float DistanceSquared = FVector::DistSquared(FoundActor->GetActorLocation(), GetActorLocation());
				if (DistanceSquared <= ClosestDistanceSquared)
				{
					ClosestDistanceSquared = DistanceSquared;
					ClosestTarget = Guard;
				}
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
		if (Distance >= StoppingDistance && CombatState == ECombatState::ECS_Moving)
		{
			if (bIsAttacking) return;
			// Continue moving towards target
			bIsMovingTowardsTarget = true;
			AddMovementInput(MoveDirection, .3f);
		} 
	}
	else
	{
		// No need to continue moving towards target
		bIsMovingTowardsTarget = false;
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

void AGoblin::ReceiveDamage(float EnemyDamage)
{
	CurrentHealth -= EnemyDamage;
	if (CurrentHealth <= 0.f)
	{
		CurrentHealth = 0;
		bIsDead = true;
		CombatState = ECombatState::ECS_Dead;
		Dead();
	}
}

void AGoblin::AttackTarget()
{
	//Gets all overlapping actors
	TSet<AActor*> WeaponOverlappingActors;
	Weapon->GetOverlappingActors(WeaponOverlappingActors);
	for (AActor* HitActor : WeaponOverlappingActors)
	{
		if (HitActor == this)
		{
			continue;
		}
		FString ActorName = HitActor->GetActorNameOrLabel();
		if (ActorName.Contains("BP_Projectile_GreenEnergy"))
		{
			continue;
		}
		if (!AttackHitActors.Contains(HitActor) && Cast<AGuardianOfTheRealmCharacter>(HitActor))
		{
			AttackHitActors.Add(HitActor);
			// Casts into AGuardianOfTheRealmCharacter to receive damage
			AGuardianOfTheRealmCharacter* Guardian = Cast<AGuardianOfTheRealmCharacter>(HitActor);
			Guardian->ReceiveDamage(Damage);
		}
		else if (!AttackHitActors.Contains(HitActor) && Cast<AGuard>(HitActor))
		{
			AttackHitActors.Add(HitActor);
			// Casts into AGuard to receive damage
			AGuard* Guard = Cast<AGuard>(HitActor);
			Guard->ReceiveDamage(Damage);
		}
	}

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
		// If the enemy and the goblin are staying still, it doesnt play the attack montage
		CombatState = ECombatState::ECS_Attacking;
		bIsAttacking = true;
		bIsMovingTowardsTarget = false;
	}

}

void AGoblin::EndAttack()
{	
	AttackHitActors.Empty();
	CurrentTarget = GetClosestTarget();
	
	if (CurrentTarget)
	{
		// Sets new rotation to the goblin to face the CurrentTarget
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(CurrentTarget->GetActorLocation() - GetActorLocation()).Rotator();
		SetActorRotation(LookAtRotation);
		UE_LOG(LogTemp, Display, TEXT("Attack Ended"));
		
		bIsAttacking = false;
		bIsMovingTowardsTarget = false;
		//Checks Distance between goblin and CurrentTarget 
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
	CurrentTarget = GetClosestTarget();
}

void AGoblin::GoblinLogic()
{
	if (CombatState == ECombatState::ECS_Dead) return;
	if (CurrentTarget)
	{
		float DistanceToTarget = FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation());

		if (DistanceToTarget <= AttackRange)
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
		CombatState = ECombatState::ECS_Idle;
	}
	
	if (CombatState == ECombatState::ECS_Idle)
	{
		Idle();
	}
}

void AGoblin::Dead()
{
	bIsAttacking = false;
	bIsMovingTowardsTarget = false;
}

// Called every frame
void AGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GoblinLogic();

}
