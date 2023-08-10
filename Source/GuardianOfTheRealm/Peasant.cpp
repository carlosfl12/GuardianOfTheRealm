// Fill out your copyright notice in the Description page of Project Settings.


#include "Peasant.h"

// Sets default values
APeasant::APeasant()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void APeasant::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void APeasant::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	MoveForward();
}

// Called to bind functionality to input
void APeasant::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

void APeasant::MoveForward()
{
	FRotator LookAtRotation = FRotationMatrix::MakeFromX(Target->GetActorLocation() - GetActorLocation()).Rotator();
	SetActorRotation(LookAtRotation);

	FVector MoveDirection = Target->GetActorLocation() - GetActorLocation();
	MoveDirection.Normalize();
	AddMovementInput(MoveDirection, .7f);
}

