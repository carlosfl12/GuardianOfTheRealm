// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Peasant.generated.h"

UCLASS()
class GUARDIANOFTHEREALM_API APeasant : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	APeasant();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void MoveForward();

private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Movement", meta = (AllowPrivateAccess = "true"))
	AActor* Target;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
