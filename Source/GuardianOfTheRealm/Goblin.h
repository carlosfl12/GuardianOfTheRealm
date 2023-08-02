// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "Goblin.generated.h"

UENUM(BlueprintType)
enum class ECombatState : uint8
{
	ECS_Idle UMETA(DisplayName="Idle"),
	ECS_Attacking UMETA(DisplayName="Attacking"),
	ECS_Moving UMETA(DisplayName="Moving"),
	ECS_Running UMETA(DisplayName="Running"),

	ECS_MAX UMETA(DisplayName="DefaultMAX")
};


UCLASS()
class GUARDIANOFTHEREALM_API AGoblin : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGoblin();

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
	void AttackTarget();
	void EndAttack();

	void Idle();
	void GoblinLogic();

private:
	FTimerHandle AttackTimerHandle;
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float AttackDuration = 1.5f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	float Speed;

	/** Distance where the goblin has to stop*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float StoppingDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	class AGuardianOfTheRealmCharacter* CurrentTarget;
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsMovingTowardsTarget;

	/** Combat State, can only Move if bIsAttacking is false*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	AGuardianOfTheRealmCharacter* RealmCharacter;
	AGuardianOfTheRealmCharacter* GetRealmCharacter() const;
	AGuardianOfTheRealmCharacter* GetClosestTarget(AActor* Caller) const;

	void MoveToPlayer();

	bool GetIsMovingTowardsTarget(AActor* Target, float MaxDistance);

	FORCEINLINE bool GetIsMovingTowardsTarget() const { return bIsMovingTowardsTarget;}
	FORCEINLINE bool GetIsAttacking() const { return bIsAttacking; }

};
