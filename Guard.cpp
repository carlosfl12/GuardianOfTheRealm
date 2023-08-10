// Fill out your copyright notice in the Description page of Project Settings.


#include "Guard.h"
#include "Kismet/GameplayStatics.h"
#include "Goblin.h"

// Sets default values
AGuard::AGuard() :
	CurrentHealth(100.f),
	MaxHealth(100.f),
	Damage(20.f),
	CombatState(ECombatState::ECS_Idle),
	bIsAttacking(false),
	bIsDead(false),
	bIsMovingTowardsTarget(false),
	bIsInjured(false),
	CurrentTarget(nullptr)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGuard::BeginPlay()
{
	Super::BeginPlay();
	
	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	Weapon = AttachedActors[0];
}

void AGuard::ReceiveDamage(float EnemyDamage)
{
	CurrentHealth -= EnemyDamage;
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		bIsDead = true;
		CombatState = ECombatState::ECS_Dead;
		Dead();
	}
}

void AGuard::AttackTarget()
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
		if (!AttackHitActors.Contains(HitActor) && Cast<AGoblin>(HitActor))
		{
			AttackHitActors.Add(HitActor);
			// Casts into AGuardianOfTheRealmCharacter to receive damage
			AGoblin* Goblin = Cast<AGoblin>(HitActor);
			Goblin->ReceiveDamage(Damage);
			UE_LOG(LogTemp, Display, TEXT("%s Health:  %f"), *Goblin->GetActorNameOrLabel(), Goblin->GetCurrentHealth());
		}
	}

	CombatState = ECombatState::ECS_Attacking;
	bIsAttacking = true;
	bIsMovingTowardsTarget = false;
}

void AGuard::EndAttack()
{
	AttackHitActors.Empty();
	CurrentTarget = GetClosestGoblin();
	
	if (CurrentTarget)
	{
		
		UE_LOG(LogTemp, Display, TEXT("%s, Attack Ended "), *GetActorNameOrLabel());
		
		bIsAttacking = false;
		bIsMovingTowardsTarget = false;
		//Checks Distance between goblin and CurrentTarget 
		float Distance = FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation());

		if (Distance > AttackDistance && Distance < DistanceDetection)
		{
			CombatState = ECombatState::ECS_Moving;
		} 
		else
		{
			CombatState = ECombatState::ECS_Idle;
		}
		
	}
}

void AGuard::Dead()
{
	bIsMovingTowardsTarget = false;
	bIsAttacking = false;
}

AGoblin *AGuard::GetClosestGoblin() const
{
    TArray<AActor*> FoundActors; 
    UGameplayStatics::GetAllActorsOfClass(GetWorld(), AGoblin::StaticClass(), FoundActors);

    float ClosestDistanceSquared = DistanceDetection * DistanceDetection;
    AGoblin* ClosestGoblin = nullptr;

    for (AActor* FoundActor : FoundActors)
    {

        if (AGoblin* Goblin = Cast<AGoblin>(FoundActor))
        {
			if (Goblin->GetIsDead())
			{
				continue;
			}
            float DistanceSquared = FVector::DistSquared(FoundActor->GetActorLocation(), GetActorLocation());
            if (DistanceSquared <= ClosestDistanceSquared)
            {
                ClosestDistanceSquared = DistanceSquared;
                ClosestGoblin = Goblin;
            }
        }
    }

    return ClosestGoblin;
}

void AGuard::GuardLogic()
{
	if (CombatState == ECombatState::ECS_Dead) return;
	CurrentTarget = GetClosestGoblin();
	if (CurrentTarget)
	{
		float Distance = FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation());

		if (Distance <= AttackDistance)
		{
			AttackTarget();
		}
		else if (GetIsMovingTowardsTarget(CurrentTarget, DistanceDetection))
		{

			MoveTowardsTarget();
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

void AGuard::Idle()
{
	bIsAttacking = false;
	bIsMovingTowardsTarget = false;
}

void AGuard::MoveTowardsTarget()
{
	if (CurrentTarget)
	{	
		//Rotate towards target
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(CurrentTarget->GetActorLocation() - GetActorLocation()).Rotator();
		SetActorRotation(LookAtRotation);

		FVector MoveDirection = CurrentTarget->GetActorLocation() - GetActorLocation();
		MoveDirection.Normalize();

		float Distance = FVector::Distance(GetActorLocation(), CurrentTarget->GetActorLocation());
		// UE_LOG(LogTemp, Display, TEXT("Distance towards %s: %f"), *CurrentTarget->GetActorNameOrLabel(), Distance);
		if (Distance >= StoppingDistance && CombatState == ECombatState::ECS_Moving)
		{
			if (bIsAttacking) return;
			// Continue moving towards target
			bIsMovingTowardsTarget = true;
			AddMovementInput(MoveDirection, Speed);
		} 
	}
	else
	{
		// No need to continue moving towards target
		bIsMovingTowardsTarget = false;
	}

	if ((MaxHealth * CurrentHealth) / 100.f <= 50.f)
	{
		bIsInjured = true;
		/* TODO: Return to base or something*/
		CombatState = ECombatState::ECS_Moving;
	}
	else
	{
		bIsInjured = false;
	}
}

bool AGuard::GetIsMovingTowardsTarget(AActor *Target, float MaxDistance)
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
	// If there isn't target, stay idle
	CombatState = ECombatState::ECS_Idle;
    return false;
}

void AGuard::RotateTowardsTarget()
{
	// Sets new rotation to the goblin to face the CurrentTarget
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(CurrentTarget->GetActorLocation() - GetActorLocation()).Rotator();
	SetActorRotation(LookAtRotation);
}

// Called every frame
void AGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	GuardLogic();

}

