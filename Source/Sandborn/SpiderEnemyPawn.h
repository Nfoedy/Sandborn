#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "SpiderEnemyPawn.generated.h"


class USphereComponent;
class UBoxComponent;
class USceneComponent;
class USkeletalMeshComponent;
class UFloatingPawnMovement;
class UPawnMovementComponent;
class UAnimMontage;
class ACollectableKey;


UCLASS()
class SANDBORN_API ASpiderEnemyPawn : public APawn
{
	GENERATED_BODY()


public:

	ASpiderEnemyPawn();

	virtual void Tick(float DeltaTime) override;

	virtual UPawnMovementComponent* GetMovementComponent() const override;

	virtual void HandleStomp();

	// Attack
	void StartAttack();
	void PerformAttackHit();
	void EndAttack();
	bool IsAttacking() const { return bIsAttacking; }

	// Death
	void Die();
	void CompleteDeath();
	void FreezeDeathPose();
	void DropKey();


private:


	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USphereComponent* MovementCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* CollisionBox;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UBoxComponent* StompCollision;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* MeshPivot;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USkeletalMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UFloatingPawnMovement* MovementComponent;


	// Movements
	UPROPERTY(EditAnywhere)
	float RotationSpeed;


	// Ground Check
	void KeepGrounded(float DeltaTime);
	void UpdateGroundAlignment(float DeltaTime);

	UPROPERTY(EditAnywhere)
	float GroundCheckDistance;

	UPROPERTY(EditAnywhere)
	float GroundSnapSpeed;

	UPROPERTY(EditAnywhere)
	float GroundAlignDistance;

	UPROPERTY(EditAnywhere)
	float GroundAlignSpeed;

	UPROPERTY(EditAnywhere)
	float MaxGroundPitch;


	// Attack
	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackMontage;

	UPROPERTY(EditAnywhere)
	float AttackDamage;

	UPROPERTY(EditAnywhere)
	float AttackHitRange;

	UPROPERTY(EditAnywhere)
	float AttackCooldown;

	bool bIsAttacking;
	bool bCanAttack;

	FTimerHandle AttackCooldownTimer;

	void ResetAttackCooldown();


	// Stomp
	UFUNCTION()
	void OnStompOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);


	// Death
	UPROPERTY(EditAnywhere, Category = "Spider|Death")
	UAnimMontage* DeathMontage;

	bool bIsDead;


	// Drop
	UPROPERTY(EditAnywhere, Category = "Spider|Drop")
	TSubclassOf<ACollectableKey> DroppedKeyClass;

	UPROPERTY(EditAnywhere, Category = "Spider|Drop")
	FVector KeyDropOffset;

	bool bHasDroppedKey;
};