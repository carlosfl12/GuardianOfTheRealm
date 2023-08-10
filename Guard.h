// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "CombatType.h"
#include "Guard.generated.h"

UCLASS()
class GUARDIANOFTHEREALM_API AGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AGuard();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void AttackTarget();

	UFUNCTION(BlueprintCallable)
	void EndAttack();

	class AGoblin* GetClosestGoblin() const;

	void GuardLogic();

	// Sets combat bools to false
	void Idle();

	void Dead();

	void MoveTowardsTarget();

	bool GetIsMovingTowardsTarget(AActor* Target, float MaxDistance);
	UFUNCTION(BlueprintCallable)
	void RotateTowardsTarget();

private:
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	AActor* Weapon;

	/** Min 0 Max 1*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	float Speed;

	/** Used to stop duplicate hits */
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	TArray<AActor*> AttackHitActors;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	float CurrentHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	float MaxHealth;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	float Damage;

	/** Distance where the Guard should detect the enemy*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	float DistanceDetection = 500.f;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	float AttackDistance = 200.f;
	/** Distance where the Guard should stop and attack*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category="Combat", meta = (AllowPrivateAccess = "true"))
	float StoppingDistance = 200.f;

	/** Combat State, can only Move if bIsAttacking is false*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category ="Combat", meta = (AllowPrivateAccess = "true"))
	ECombatState CombatState;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsAttacking;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsMovingTowardsTarget;

	/** If CurrentHealth is <= 50%*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Combat", meta = (AllowPrivateAccess = "true"))
	bool bIsInjured;

	/** CurrentTarget of the guard*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Combat", meta = (AllowPrivateAccess = "true"))
	AGoblin* CurrentTarget;

	/** Last target of the guard*/
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category ="Combat", meta = (AllowPrivateAccess = "true"))
	AGoblin* LastTarget;
public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable)
	void ReceiveDamage(float EnemyDamage);

	FORCEINLINE bool GetIsDead() const { return bIsDead; }
	FORCEINLINE float GetCurrentHealth() const { return CurrentHealth; }
	FORCEINLINE bool GetIsMovingTowardsTarget() const { return bIsMovingTowardsTarget; }
	FORCEINLINE bool GetIsAttacking() const { return bIsAttacking; }
	FORCEINLINE bool GetIsInjured() const { return bIsInjured; }

};
