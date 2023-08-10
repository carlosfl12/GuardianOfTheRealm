// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatType.h"
#include "Goblin.generated.h"

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
	virtual void AttackTarget();
	UFUNCTION(BlueprintCallable)
	virtual void EndAttack();

	void Idle();
	void GoblinLogic();
	void Dead();

private:
	// FTimerHandle AttackTimerHandle;
	// UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	// float AttackDuration = .03f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	float Speed;

	/** Distance where the goblin has to stop*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float StoppingDistance;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AActor* CurrentTarget; // Change to AActor and then cast into guard or player
		
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsMovingTowardsTarget;

	/** Combat State, can only Move if bIsAttacking is false*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	/** Max Health of the Goblin*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float MaxHealth;
	/** Current Health of the Goblin*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;
	/** Base Damage dealt by the goblin*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	float Damage;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	bool bIsDead;


	/** Weapon of the goblin*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Combat, meta = (AllowPrivateAccess = "true"))
	AActor* Weapon;

	// Used to stop duplicate hits
	TArray<AActor*> AttackHitActors;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	class AGuardianOfTheRealmCharacter* RealmCharacter;
	AGuardianOfTheRealmCharacter* GetRealmCharacter() const;
	AActor* GetClosestTarget() const; // Change it to AActor
	void MoveToPlayer();

	bool GetIsMovingTowardsTarget(AActor* Target, float MaxDistance);

	FORCEINLINE bool GetIsMovingTowardsTarget() const { return bIsMovingTowardsTarget;}
	FORCEINLINE bool GetIsAttacking() const { return bIsAttacking; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	void ReceiveDamage(float EnemyDamage);


};
