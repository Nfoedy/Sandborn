#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "SpiderAIController.generated.h"


class UAIPerceptionComponent;
class UAISenseConfig_Sight;


UCLASS()
class SANDBORN_API ASpiderAIController : public AAIController
{
	GENERATED_BODY()

	ASpiderAIController();

protected:

	virtual void BeginPlay() override;
	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result) override;


private:

	// Patrol System
	void TryStartPatrol();
	void MoveToRandomPoint();

	FTimerHandle NavigationRetryTimer;
	FTimerHandle PatrolTimer;
	int32 NavigationRetryCount;

	UPROPERTY(EditAnywhere)
	float PatrolRadius;

	UPROPERTY(EditAnywhere)
	float PatrolWaitTime;

	
	// Stuck check
	void StartStuckCheck();
	void CheckIfStuck();

	FTimerHandle StuckCheckTimer;
	FVector LastCheckedLocation;
	float CurrentStuckTime;
	bool bRecoveringFromStuck;

	UPROPERTY(EditAnywhere)
	float MaxStuckTime;

	UPROPERTY(EditAnywhere)
	float StuckDistanceThreshold;

	// AI Perception
	UPROPERTY(VisibleAnywhere)
	UAIPerceptionComponent* AIPerception;

	UPROPERTY()
	UAISenseConfig_Sight* SightConfig;


	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);


	// Chasing
	bool bIsChasing;

	AActor* CurrentTarget;

	FTimerHandle ChaseTimer;

	UPROPERTY(EditAnywhere, Category = "AI|Combat")
	float AttackRange;

	void UpdateChase();


	// Attack

	UPROPERTY(EditAnywhere)
	float MoveAcceptanceRadius;


};