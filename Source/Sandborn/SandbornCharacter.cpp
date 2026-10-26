#include "SandbornCharacter.h"
#include "Sandborn.h"
#include "CollectableKeyHolder.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/Controller.h"
#include "EnhancedInputComponent.h"
#include "CollectableKey.h"
#include "InputActionValue.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/Engine.h"


ASandbornCharacter::ASandbornCharacter()
{
	GetCapsuleComponent()->InitCapsuleSize(42.0f, 96.0f);

	bUseControllerRotationPitch = false;
	bUseControllerRotationYaw = false;
	bUseControllerRotationRoll = false;
	
	// Movement
	GetCharacterMovement()->bOrientRotationToMovement = true;
	GetCharacterMovement()->RotationRate = FRotator(0.0f, 500.0f, 0.0f);
	GetCharacterMovement()->JumpZVelocity = 500.0f;
	GetCharacterMovement()->AirControl = 0.35f;
	GetCharacterMovement()->MaxWalkSpeed = 500.0f;
	GetCharacterMovement()->MinAnalogWalkSpeed = 20.0f;
	GetCharacterMovement()->BrakingDecelerationWalking = 2000.0f;
	GetCharacterMovement()->BrakingDecelerationFalling = 1500.0f;

	// Double Jump
	JumpMaxCount = 2;

	// Camera
	CameraBoom = CreateDefaultSubobject<USpringArmComponent>(TEXT("CameraBoom"));
	CameraBoom->SetupAttachment(RootComponent);
	CameraBoom->TargetArmLength = 400.0f;
	CameraBoom->bUsePawnControlRotation = true;
	FollowCamera = CreateDefaultSubobject<UCameraComponent>(TEXT("FollowCamera"));
	FollowCamera->SetupAttachment(CameraBoom, USpringArmComponent::SocketName);
	FollowCamera->bUsePawnControlRotation = false;

	// Coins
	CollectedCoins = 0;

	// Keys
	CollectedKeys = 0;
	MaxKeys = 3;

	// Health
	Health = 3;
	MaxHealth = 5;

	// Death
	IsDead = false;
	GameOverDelay = 2.0f;
}


void ASandbornCharacter::BeginPlay()
{
	Super::BeginPlay();

	TArray<AActor*> FoundHolders;

	UGameplayStatics::GetAllActorsOfClass(GetWorld(), ACollectableKeyHolder::StaticClass(), FoundHolders);

	for (AActor* Actor : FoundHolders)
	{
		if (ACollectableKeyHolder* Holder = Cast<ACollectableKeyHolder>(Actor))
		{
			KeyHolders.Add(Holder);
		}
	}
}



void ASandbornCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		// Jump
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Started, this, &ASandbornCharacter::DoJumpStart);
		EnhancedInputComponent->BindAction(JumpAction, ETriggerEvent::Completed, this, &ACharacter::StopJumping);

		// Movement
		EnhancedInputComponent->BindAction(MoveAction, ETriggerEvent::Triggered, this, &ASandbornCharacter::Move);

		// Mouse Look
		EnhancedInputComponent->BindAction(MouseLookAction, ETriggerEvent::Triggered, this, &ASandbornCharacter::Look);

		// Controller Look
		EnhancedInputComponent->BindAction(LookAction, ETriggerEvent::Triggered, this, &ASandbornCharacter::Look);

	}
	else
	{
		UE_LOG(LogSandborn,	Error, TEXT("'%s' Failed to find an Enhanced Input component!"), *GetNameSafe(this));
	}
}


void ASandbornCharacter::Move(const FInputActionValue& Value)
{
	const FVector2D MovementVector = Value.Get<FVector2D>();

	DoMove(MovementVector.X, MovementVector.Y);
}


void ASandbornCharacter::Look(const FInputActionValue& Value)
{
	const FVector2D LookAxisVector = Value.Get<FVector2D>();

	DoLook(LookAxisVector.X, LookAxisVector.Y);
}


void ASandbornCharacter::DoMove(float Right, float Forward)
{
	if (IsDead) return;

	AController* CurrentController = GetController();
	if (!Controller) return;

	const FRotator Rotation = CurrentController->GetControlRotation();
	const FRotator YawRotation(0.0f, Rotation.Yaw, 0.0f);
	const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);
	const FVector RightDirection =	FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

	AddMovementInput(ForwardDirection, Forward);
	AddMovementInput(RightDirection, Right);
}


void ASandbornCharacter::DoLook(float Yaw, float Pitch)
{
	if (!GetController()) return;

	AddControllerYawInput(Yaw);

	AddControllerPitchInput(Pitch);
}


void ASandbornCharacter::DoJumpStart()
{
	if (IsDead) return;

	Jump();
}


void ASandbornCharacter::DoJumpEnd()
{
	StopJumping();
}



void ASandbornCharacter::AddCoin()
{
	CollectedCoins++;

	if (CollectedCoins % 10 == 0 && Health < MaxHealth)
	{
		Health++;

		OnHealthChanged.Broadcast(Health, MaxHealth);
	}

	OnCoinsChanged.Broadcast(CollectedCoins);
}



void ASandbornCharacter::AddKey(int32 KeyIndex, UStaticMesh* KeyMesh, UClass* KeyClass)
{
	if (CollectedKeyIndices.Contains(KeyIndex))
	{
		return;
	}

	CollectedKeyIndices.Add(KeyIndex);

	if (KeyClass)
	{
		OnKeyCollected.Broadcast(KeyClass);
	}

	if (CollectedKeys >= MaxKeys) return;

	CollectedKeys++;

	OnKeysChanged.Broadcast(CollectedKeys, MaxKeys);

	for (ACollectableKeyHolder* Holder : KeyHolders)
	{
		if (Holder && Holder->HolderIndex == CollectedKeys)
		{
			Holder->ActivateKey(KeyMesh);
			break;
		}
	}
}



void ASandbornCharacter::LoseHealth()
{
	if (Health <= 0) return;

	Health--;

	OnHealthChanged.Broadcast(Health, MaxHealth);

	if (Health == 0)
	{
		Die();
	}

}


float ASandbornCharacter::TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	if (IsDead) return 0.0f;

	const float ActualDamage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);

	if (ActualDamage <= 0.0f) return 0.0f;

	const int32 DamageToHealth = FMath::RoundToInt(ActualDamage);

	Health = FMath::Clamp(Health - DamageToHealth, 0, MaxHealth);

	OnHealthChanged.Broadcast(Health, MaxHealth);

	if (Health <= 0)
	{
		Die();
	}

	return ActualDamage;
}



void ASandbornCharacter::Die()
{
	if (IsDead) return;

	IsDead = true;

	// Stop Character
	GetCharacterMovement()->StopMovementImmediately();
	GetCharacterMovement()->DisableMovement();
	GetCapsuleComponent()->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Ragdoll
	GetMesh()->SetCollisionProfileName(TEXT("Ragdoll"));
	GetMesh()->SetSimulatePhysics(true);
	GetMesh()->WakeAllRigidBodies();


	// Game Over Delay
	GetWorldTimerManager().SetTimer(GameOverTimer, this, &ASandbornCharacter::FinishDeath, GameOverDelay, false);

}


void ASandbornCharacter::FinishDeath()
{
	UGameplayStatics::SetGamePaused(this, true);
}


int32 ASandbornCharacter::GetHealth() const
{
	return Health;
}


int32 ASandbornCharacter::GetCoins() const
{
	return CollectedCoins;
}


void ASandbornCharacter::RestoreSavedStats(int32 SavedHealth, int32 SavedCoins)
{
	Health = FMath::Clamp(	SavedHealth, 0,	MaxHealth);

	CollectedCoins = FMath::Max(0, SavedCoins);

	OnHealthChanged.Broadcast(Health, MaxHealth);
	OnCoinsChanged.Broadcast(CollectedCoins);
}

const TArray<int32>& ASandbornCharacter::GetCollectedKeyIndices() const
{
	return CollectedKeyIndices;
}

void ASandbornCharacter::RestoreCollectedKeys(const TArray<int32>& SavedKeyIndices)
{
	CollectedKeyIndices.Empty();
	CollectedKeys = 0;

	for (int32 SavedKeyIndex : SavedKeyIndices)
	{
		for (const TSubclassOf<ACollectableKey>& KeyClass : KeyClasses)
		{
			if (!KeyClass) continue;

			const ACollectableKey* KeyDefaults = KeyClass->GetDefaultObject<ACollectableKey>();
			if (!KeyDefaults) continue;

			if (KeyDefaults->GetKeyIndex() == SavedKeyIndex)
			{
				AddKey(SavedKeyIndex, KeyDefaults->GetKeyMesh(), KeyClass.Get());
				break;
			}
		}
	}

	OnKeysChanged.Broadcast(CollectedKeys, MaxKeys);
}


int32 ASandbornCharacter::GetMaxHealth() const
{
	return MaxHealth;
}


int32 ASandbornCharacter::GetCollectedKeys() const
{
	return CollectedKeys;
}


int32 ASandbornCharacter::GetMaxKeys() const
{
	return MaxKeys;
}