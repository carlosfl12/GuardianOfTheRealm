// Copyright Epic Games, Inc. All Rights Reserved.

#include "GuardianOfTheRealmCharacter.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/Controller.h"
#include "GameFramework/SpringArmComponent.h"
#include "EnhancedInputComponent.h"
#include "EnhancedInputSubsystems.h"
#include "Goblin.h"


//////////////////////////////////////////////////////////////////////////
// AGuardianOfTheRealmCharacter

AGuardianOfTheRealmCharacter::AGuardianOfTheRealmCharacter() :
	// Stats
	MaxHealth(100.f),
	CurrentHealth(100.f),
	Damage(30.f),
	bIsDead(false),
	bCanDealDamage(false)
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(42.f, 96.0f);
		
	// Don't rotate when the controller rotates. Let that just affect the camera.
	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;

	// Configure character movement
	GetCharacterMovement()->bOrientRotationToMovement = true; // Character moves in the direction of input...	
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f); // ...at this rotation rate

	// Note: For faster iteration times these variables, and many more, can be tweaked in the Character Blueprint
	// instead of recompiling to adjust them
	GetCharacterMovement()->JumpZVelocity = 700.f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.f;

	// Create a camera boom (pulls in towards the player if there is a collision)
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f; // The camera follows at this distance behind the character	
	CameraBoom->bUsePawnControlRotation = true; // Rotate the arm based on the controller

	// Create a follow camera
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName); // Attach the camera to the end of the boom and let the boom adjust to match the controller orientation
	FollowCamera->bUsePawnControlRotation = false; // Camera does not rotate relative to arm

	// Note: The skeletal mesh and anim blueprint references on the Mesh component (inherited from Character) 
	// are set in the derived blueprint asset named ThirdPersonCharacter (to avoid direct content references in C++)
}

void AGuardianOfTheRealmCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();

	//Add Input Mapping Context
	if (APlayerController* PlayerController = Cast<APlayerController>(Controller))
	{
		if (UEnhancedInputLocalPlayerSubsystem* Subsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(PlayerController->GetLocalPlayer()))
		{
			Subsystem->AddMappingContext(DefaultMappingContext, 0);
		}
	}

	TArray<AActor*> AttachedActors;
	GetAttachedActors(AttachedActors);
	Weapon = AttachedActors[0];
}

void AGuardianOfTheRealmCharacter::SetIsAttacking(bool Value)
{
    bIsAttacking = Value;
}

void AGuardianOfTheRealmCharacter::SetCanDealDamage(bool Value)
{
	bCanDealDamage = Value;
}

void AGuardianOfTheRealmCharacter::ReceiveDamage(float EnemyDamage)
{
	if (CurrentHealth <= 0)
	{
		CurrentHealth = 0;
		bIsDead = true;
	}
	else
	{
		CurrentHealth -= EnemyDamage;
	}
}

//////////////////////////////////////////////////////////////////////////
// Input

void AGuardianOfTheRealmCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up action bindings
	if (UEnhancedInputComponent* EnhancedInputComponent = CastChecked<UEnhancedInputComponent>(PlayerInputComponent)) {
		
		//Jumping
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Triggered, this, &ACharacter::Jump);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		//Moving
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &AGuardianOfTheRealmCharacter::Move);

		//Looking
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &AGuardianOfTheRealmCharacter::Look);

		EnhancedInputComponent->BindAction(AttackAction, ETriggerEvent::Triggered, this, &AGuardianOfTheRealmCharacter::Attack);

	}

}

void AGuardianOfTheRealmCharacter::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	
		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGuardianOfTheRealmCharacter::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGuardianOfTheRealmCharacter::Attack()
{
	// if (bIsAttacking) return; 
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
		if (!AttackHitActors.Contains(HitActor) && Cast<AGoblin>(HitActor) && bCanDealDamage)
		{
			UE_LOG(LogTemp, Display, TEXT("Hitting: %s"), *HitActor->GetActorNameOrLabel());
			AttackHitActors.Add(HitActor);
			// Casts into AGuardianOfTheRealmCharacter to receive damage
			AGoblin* Goblin = Cast<AGoblin>(HitActor);
			Goblin->ReceiveDamage(Damage);
			UE_LOG(LogTemp, Display, TEXT("%s Health:  %f"), *Goblin->GetActorNameOrLabel(), Goblin->GetCurrentHealth());
		}
	}

	/** Uses the AttackMontage created in the animation BP */
	UAnimInstance* AnimInstance = GetMesh()->GetAnimInstance();
	Speed = GetVelocity().Size();
	if (AnimInstance && AttackMontage && Speed > 0.f)
	{
		// If the player is moving, play the attack while moving animation
		AnimInstance->Montage_Play(AttackMontage);
		AnimInstance->Montage_JumpToSection(FName("Attack"));
		// TODO: Player should move forward until the end of the animation
	}
	else if (Speed <= 0.f)
	{
		// TODO: if the player does this, make the player not moving until is the attack is finished
		// If the player is not moving, play the attack without moving
		SetIsAttacking(true);
	}
}

void AGuardianOfTheRealmCharacter::EndAttack()
{
	SetIsAttacking(false);
	AttackHitActors.Empty();
	bCanDealDamage = false;

}
