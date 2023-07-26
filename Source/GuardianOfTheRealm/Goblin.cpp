// Fill out your copyright notice in the Description page of Project Settings.


#include "Goblin.h"
#include "GuardianOfTheRealmCharacter.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "GoblinAnimInstance.h"

// Sets default values
AGoblin::AGoblin()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

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

void AGoblin::MoveToPlayer()
{
	if (RealmCharacter)
	{
		FRotator LookAtRotation = FRotationMatrix::MakeFromX(RealmCharacter->GetActorLocation() - GetActorLocation()).Rotator();
		SetActorRotation(LookAtRotation);

		FVector MoveDirection = RealmCharacter->GetActorLocation() - GetActorLocation();
		MoveDirection.Normalize();
		AddMovementInput(MoveDirection, 1.0f);
	}
}
void AGoblin::AttackEnemy()
{
	/**
	 * Calls Anim
	 * Sets attack in true
	 * Needs to be refactored later
	*/
	UGoblinAnimInstance* GoblinAnim = Cast<UGoblinAnimInstance>(GoblinAnimInstance);
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();

	if (GoblinAnim && AttackMontage && AnimInstance)
	{
		// Mixes movement with the attack montage
		// Needs to attack once every X seconds
		
		GoblinAnim->SetIsAttacking(true);
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("Attack"));
	}

}

void AGoblin::Idle()
{
}

void AGoblin::GoblinLogic()
{
	UGoblinAnimInstance* GoblinAnim = Cast<UGoblinAnimInstance>(GoblinAnimInstance);
	if (RealmCharacter)
	{
		float DistanceToPlayer = FVector::Distance(GetActorLocation(), RealmCharacter->GetActorLocation());

		if (DistanceToPlayer <= AttackRange)
		{
			AttackEnemy();
		}
		else if (GoblinAnim->GetIsMovingTowardsTarget(this, RealmCharacter, DistanceDetection))
		{
			MoveToPlayer();
		}
		else
		{
			Idle();
		}
	}
}

// Called every frame
void AGoblin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	GoblinLogic();
}
