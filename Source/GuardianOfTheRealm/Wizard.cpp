// Fill out your copyright notice in the Description page of Project Settings.


#include "Wizard.h"
#include "GoblinAnimInstance.h"

void AWizard::AttackTarget()
{
    Super::AttackTarget();
 
}

void AWizard::BeginPlay()
{
    Super::BeginPlay();
    GoblinAnim = Cast<UGoblinAnimInstance>(GetMesh()->GetAnimInstance());
    GoblinAnim->SetWizard(this);
}

void AWizard::EndCast()
{
    ProjectileSpawnPosition = GetMesh()->GetSocketLocation("SpawnSocket");
    AGoblin::EndAttack();
    if (ParticleActor)
    {
        // Spawns a projectile in the Socket position
        FActorSpawnParameters SpawnParams;
        SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
        GetWorld()->SpawnActor<AActor>(ParticleActor, ProjectileSpawnPosition, GetActorRotation(), SpawnParams);
    }
}
