#include "SpiderEnemyPawn.h"
#include "SpiderAIController.h"
#include "CollectableKey.h"
#include "Components/SphereComponent.h"
#include "Components/BoxComponent.h"
#include "Components/SceneComponent.h"
#include "Components/SkeletalMeshComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "GameFramework/Character.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Animation/AnimInstance.h"
#include "Animation/AnimMontage.h"
#include "Kismet/GameplayStatics.h"
#include "AIController.h"
#include "Engine/Engine.h"
#include "TimerManager.h"


ASpiderEnemyPawn::ASpiderEnemyPawn()
{
	PrimaryActorTick.bCanEverTick = true;
	
	// Controller
	AIControllerClass = ASpiderAIController::StaticClass();
	AutoPossessAI = EAutoPossessAI::PlacedInWorldOrSpawned;

	// Movement
	MovementCollision = CreateDefaultSubobject<USphereComponent>(TEXT("MovementCollision"));
	SetRootComponent(MovementCollision);
	MovementCollision->InitSphereRadius(55.0f);
	MovementCollision->SetCollisionProfileName(TEXT("Pawn"));
	MovementCollision->SetCanEverAffectNavigation(false);

	// Gameplay Collision
	CollisionBox = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionBox"));
	CollisionBox->SetupAttachment(MovementCollision);
	CollisionBox->SetBoxExtent(FVector(100.0f, 60.0f, 35.0f));
	CollisionBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	CollisionBox->SetCollisionResponseToAllChannels(ECR_Overlap);
	CollisionBox->SetCanEverAffectNavigation(false);

	// Stomp
	StompCollision = CreateDefaultSubobject<UBoxComponent>(TEXT("StompCollision"));
	StompCollision->SetupAttachment(MovementCollision);
	StompCollision->SetBoxExtent(FVector(60.0f, 50.0f, 15.0f));
	StompCollision->SetRelativeLocation(FVector(0.0f, 0.0f, 55.0f));
	StompCollision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	StompCollision->SetCollisionResponseToAllChannels(ECR_Ignore);
	StompCollision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	StompCollision->SetCanEverAffectNavigation(false);
	StompCollision->OnComponentBeginOverlap.AddDynamic(this, &ASpiderEnemyPawn::OnStompOverlap);

	
	// Mesh
	MeshPivot = CreateDefaultSubobject<USceneComponent>(TEXT("MeshPivot"));
	MeshPivot->SetupAttachment(MovementCollision);
	Mesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(MeshPivot);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Mesh->SetCanEverAffectNavigation(false);

	// Movement
	MovementComponent =	CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("MovementComponent"));
	MovementComponent->SetUpdatedComponent(MovementCollision);
	MovementComponent->MaxSpeed = 300.0f;
	MovementComponent->Acceleration = 800.0f;
	MovementComponent->Deceleration = 800.0f;
	MovementComponent->NavAgentProps.AgentRadius = 55.0f;
	MovementComponent->NavAgentProps.AgentHeight = 110.0f;
	MovementComponent->NavAgentProps.bCanWalk = true;

	// Speed Rotation
	RotationSpeed = 5.0f;

	// Ground
	GroundCheckDistance = 200.0f;
	GroundSnapSpeed = 15.0f;
	GroundAlignDistance = 80.0f;
	GroundAlignSpeed = 6.0f;
	MaxGroundPitch = 45.0f;

	// Attack
	AttackMontage = nullptr;
	AttackDamage = 1.0f;
	AttackHitRange = 170.0f;
	AttackCooldown = 1.0f;
	bIsAttacking = false;
	bCanAttack = true;


	// Death
	DeathMontage = nullptr;
	bIsDead = false;
	SaveId = 0;


	// Drop
	DroppedKeyClass = nullptr;
	KeyDropOffset = FVector(0.0f, 0.0f, 70.0f);
	bHasDroppedKey = false;
}



UPawnMovementComponent* ASpiderEnemyPawn::GetMovementComponent() const
{
	return MovementComponent;
}



void ASpiderEnemyPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bIsDead) return;

	// Rotate Spider towards movement direction
	FVector Velocity = GetVelocity();
	Velocity.Z = 0.0f;

	if (!Velocity.IsNearlyZero())
	{
		const FRotator TargetRotation = Velocity.Rotation();
		const FRotator NewRotation = FMath::RInterpTo(GetActorRotation(), TargetRotation, DeltaTime, RotationSpeed);

		SetActorRotation(NewRotation);
	}

	KeepGrounded(DeltaTime);

	UpdateGroundAlignment(DeltaTime);
}



void ASpiderEnemyPawn::KeepGrounded(float DeltaTime)
{
	if (!MovementCollision) return;

	const FVector Start = GetActorLocation() + FVector(0.0f, 0.0f, 80.0f);
	const FVector End = GetActorLocation() - FVector(0.0f, 0.0f, GroundCheckDistance);

	FHitResult Hit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);

	const bool bHitGround = GetWorld()->LineTraceSingleByChannel(Hit, Start, End, ECC_Visibility, Params);
	if (!bHitGround) return;

	const float SphereRadius = MovementCollision->GetScaledSphereRadius();

	const float TargetZ = Hit.ImpactPoint.Z + SphereRadius;

	FVector NewLocation = GetActorLocation();

	NewLocation.Z = FMath::FInterpTo(NewLocation.Z, TargetZ, DeltaTime,	GroundSnapSpeed);

	SetActorLocation(NewLocation, true);
}



void ASpiderEnemyPawn::UpdateGroundAlignment(float DeltaTime)
{
	if (!MeshPivot) return;

	const FVector Forward = GetActorForwardVector().GetSafeNormal2D();

	if (Forward.IsNearlyZero()) return;

	const FVector ActorLocation = GetActorLocation();

	const FVector FrontPoint = ActorLocation + Forward * GroundAlignDistance;
	const FVector BackPoint = ActorLocation - Forward * GroundAlignDistance;

	const float TraceHeight = 150.0f;
	const float TraceDepth = 300.0f;

	const FVector FrontStart = FrontPoint + FVector(0.0f, 0.0f, TraceHeight);
	const FVector FrontEnd = FrontPoint - FVector(0.0f, 0.0f, TraceDepth);

	const FVector BackStart = BackPoint + FVector(0.0f, 0.0f, TraceHeight);
	const FVector BackEnd = BackPoint - FVector(0.0f, 0.0f, TraceDepth);

	FHitResult FrontHit;
	FHitResult BackHit;

	FCollisionQueryParams Params;
	Params.AddIgnoredActor(this);


	const bool bFrontHit = GetWorld()->LineTraceSingleByChannel(FrontHit, FrontStart, FrontEnd, ECC_Visibility,	Params);
	const bool bBackHit = GetWorld()->LineTraceSingleByChannel(BackHit, BackStart, BackEnd, ECC_Visibility, Params);
	if (!bFrontHit || !bBackHit)
	{
		const FRotator NewRotation = FMath::RInterpTo(MeshPivot->GetRelativeRotation(), FRotator::ZeroRotator, DeltaTime, GroundAlignSpeed);

		MeshPivot->SetRelativeRotation(NewRotation);

		return;
	}


	const float HeightDifference = FrontHit.ImpactPoint.Z - BackHit.ImpactPoint.Z;
	
	const float HorizontalDistance = GroundAlignDistance * 2.0f;

	float TargetPitch = FMath::RadiansToDegrees(FMath::Atan2(HeightDifference, HorizontalDistance));

	TargetPitch = FMath::Clamp(TargetPitch, -MaxGroundPitch, MaxGroundPitch);

	const FRotator TargetRotation(TargetPitch, 0.0f, 0.0f);

	const FRotator NewRotation = FMath::RInterpTo(MeshPivot->GetRelativeRotation(), TargetRotation, DeltaTime,	GroundAlignSpeed);

	MeshPivot->SetRelativeRotation(NewRotation);
}



void ASpiderEnemyPawn::StartAttack()
{
	if (bIsAttacking || !bCanAttack) return;
	if (!AttackMontage) return;

	UAnimInstance* AnimInstance = Mesh->GetAnimInstance();

	if (!AnimInstance) return;

	bIsAttacking = true;
	bCanAttack = false;

	AnimInstance->Montage_Play(AttackMontage);

}



void ASpiderEnemyPawn::PerformAttackHit()
{
	ACharacter* PlayerCharacter = UGameplayStatics::GetPlayerCharacter(GetWorld(),	0);

	if (!PlayerCharacter) return;

	const float Distance = FVector::Dist(GetActorLocation(), PlayerCharacter->GetActorLocation());

	if (Distance > AttackHitRange) return;

	UGameplayStatics::ApplyDamage(PlayerCharacter, AttackDamage, GetController(), this, nullptr);

}



void ASpiderEnemyPawn::EndAttack()
{
	bIsAttacking = false;

	GetWorldTimerManager().SetTimer(AttackCooldownTimer, this, &ASpiderEnemyPawn::ResetAttackCooldown, AttackCooldown, false);
}



void ASpiderEnemyPawn::ResetAttackCooldown()
{
	bCanAttack = true;
}



void ASpiderEnemyPawn::OnStompOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	if (!OtherActor || OtherActor == this) return;

	ACharacter* PlayerCharacter = Cast<ACharacter>(OtherActor);
	if (!PlayerCharacter) return;

	UCharacterMovementComponent* CharacterMovement = PlayerCharacter->GetCharacterMovement();
	if (!CharacterMovement) return;
	if (!CharacterMovement->IsFalling()) return;

	if (PlayerCharacter->GetVelocity().Z >= 0.0f) return;

	if (PlayerCharacter->GetActorLocation().Z <=GetActorLocation().Z) return;

	HandleStomp();
}



void ASpiderEnemyPawn::Die()
{
	if (bIsDead) return;

	bIsDead = true;
	bIsAttacking = false;

	// Stop movement
	if (MovementComponent)
	{
		MovementComponent->StopMovementImmediately();
		MovementComponent->Deactivate();
	}

	// Stop AI
	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->StopMovement();
		AIController->UnPossess();
	}

	// Disable collisions
	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (StompCollision)
	{
		StompCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (MovementCollision)
	{
		MovementCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}


	// Death animation
	if (DeathMontage && Mesh)
	{
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}
}



void ASpiderEnemyPawn::CompleteDeath()
{
	DropKey();

	FreezeDeathPose();
}



void ASpiderEnemyPawn::FreezeDeathPose()
{
	if (!Mesh) return;

	Mesh->bPauseAnims = true;
}



void ASpiderEnemyPawn::DropKey()
{
	if (bHasDroppedKey) return;
	if (!DroppedKeyClass) return;

	bHasDroppedKey = true;

	const FVector SpawnLocation = GetActorLocation() + KeyDropOffset;

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;

	GetWorld()->SpawnActor<ACollectableKey>(DroppedKeyClass, SpawnLocation, FRotator::ZeroRotator, SpawnParams);
}



void ASpiderEnemyPawn::HandleStomp()
{
	Die();
}


int32 ASpiderEnemyPawn::GetSaveId() const
{
	return SaveId;
}


bool ASpiderEnemyPawn::IsDeadForSave() const
{
	return bIsDead;
}


void ASpiderEnemyPawn::RestoreDeadState(bool bKeyWasCollected)
{
	if (bIsDead) return;

	bIsDead = true;
	bIsAttacking = false;
	bCanAttack = false;

	bHasDroppedKey = bKeyWasCollected;

	if (MovementComponent)
	{
		MovementComponent->StopMovementImmediately();
		MovementComponent->Deactivate();
	}

	if (AAIController* AIController = Cast<AAIController>(GetController()))
	{
		AIController->StopMovement();
		AIController->UnPossess();
	}

	if (CollisionBox)
	{
		CollisionBox->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (StompCollision)
	{
		StompCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (MovementCollision)
	{
		MovementCollision->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	if (Mesh)
	{
		Mesh->bPauseAnims = false;
	}

	if (DeathMontage && Mesh)
	{
		if (UAnimInstance* AnimInstance = Mesh->GetAnimInstance())
		{
			AnimInstance->Montage_Play(DeathMontage);
		}
	}
}


int32 ASpiderEnemyPawn::GetDroppedKeyIndex() const
{
	if (!DroppedKeyClass)
	{
		return INDEX_NONE;
	}

	const ACollectableKey* KeyDefaults = DroppedKeyClass->GetDefaultObject<ACollectableKey>();

	if (!KeyDefaults)
	{
		return INDEX_NONE;
	}

	return KeyDefaults->GetKeyIndex();
}