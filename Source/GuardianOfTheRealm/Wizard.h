// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Goblin.h"
#include "Wizard.generated.h"

/**
 * 
 */
UCLASS()
class GUARDIANOFTHEREALM_API AWizard : public AGoblin
{
	GENERATED_BODY()

private:
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	TSubclassOf<AActor> ParticleActor;

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Animation", meta = (AllowPrivateAccess = "true") )
	class UGoblinAnimInstance* GoblinAnim;

	/** Position of the socket where the projectile will spawn*/
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Combat", meta = (AllowPrivateAccess = "true"))
	FVector ProjectileSpawnPosition;
protected:
	virtual void AttackTarget() override;

	virtual void BeginPlay() override;
	
	UFUNCTION(BlueprintCallable)
 	void EndCast();
};
