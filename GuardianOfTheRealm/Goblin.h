// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Goblin.generated.h"

UCLASS()
class GUARDIANOFTHEREALM_API AGoblin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGoblin();
	float TimesCounting;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	/** Anim Instance of the Goblin*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimInstance* GoblinAnimInstance;

	/** Mixes animations of Attack and Movement*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackRange = 200.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float DistanceDetection = 500.f;

	UFUNCTION(BlueprintCallable)
	void AttackEnemy();

	void Idle();
	void GoblinLogic();

private:
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class AGuardianOfTheRealmCharacter* RealmCharacter;
	AGuardianOfTheRealmCharacter* GetRealmCharacter() const;

	void MoveToPlayer();

};
