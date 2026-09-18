#include "SpiderAIController.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "NavigationSystem.h"
#include "NavigationData.h"
#include "Navigation/PathFollowingComponent.h"
#include "Engine/Engine.h"
#include "SpiderEnemyPawn.h"
#include "TimerManager.h"


ASpiderAIController::ASpiderAIController()
{
	// Patrol
	PatrolRadius = 1500.0f;
	PatrolWaitTime = 1.5f;

	// Stuck
	MaxStuckTime = 0.5f;
	StuckDistanceThreshold = 5.0f;

	// AI Perception
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	// Sight (Vista)
	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1800.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;


	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	// Chasing
	bIsChasing = false;
	CurrentTarget = nullptr;

	// Attack
	AttackRange = 150.0f;
	MoveAcceptanceRadius = 30.0f;

}


void ASpiderAIController::BeginPlay()
{
	Super::BeginPlay();

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ASpiderAIController::OnTargetPerceptionUpdated);
}


void ASpiderAIController::OnPossess(APawn* InPawn)
{
	Super::OnPossess(InPawn);
	TryStartPatrol();
}


void ASpiderAIController::TryStartPatrol()
{
	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return;

	ANavigationData* NavData = NavSystem->GetDefaultNavDataInstance(FNavigationSystem::DontCreate);
	if (!NavData)
	{
		NavigationRetryCount++;

		UE_LOG(LogTemp, Warning,TEXT("NavData non ancora pronto. Tentativo: %d"),NavigationRetryCount);

		if (NavigationRetryCount < 20)
		{
			GetWorldTimerManager().SetTimer(NavigationRetryTimer, this, &ASpiderAIController::TryStartPatrol, 0.1f, false);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("NavData non trovato dopo 20 tentativi"));
		}
		return;
	}

	NavigationRetryCount = 0;

	MoveToRandomPoint();
}


void ASpiderAIController::MoveToRandomPoint()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	UNavigationSystemV1* NavSystem = UNavigationSystemV1::GetCurrent(GetWorld());
	if (!NavSystem) return;

	FNavLocation ProjectedLocation;

	bool bOnNavMesh = NavSystem->ProjectPointToNavigation(ControlledPawn->GetActorLocation(), ProjectedLocation, FVector(300.0f, 300.0f, 500.0f));
	if (!bOnNavMesh) return;

	FNavLocation RandomLocation;

	bool bFoundLocation = NavSystem->GetRandomReachablePointInRadius(ProjectedLocation.Location, PatrolRadius, RandomLocation);
	if (!bFoundLocation) return;

	EPathFollowingRequestResult::Type Result = MoveToLocation(RandomLocation.Location, 50.0f);
	if (Result == EPathFollowingRequestResult::RequestSuccessful)
	{
		StartStuckCheck();
	}
	else if (Result == EPathFollowingRequestResult::AlreadyAtGoal)
	{
		// È uscito un punto praticamente già raggiunto, ne scegliamo un altro
		MoveToRandomPoint();
	}
	else
	{
		MoveToRandomPoint();
	}
}


void ASpiderAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);

	GetWorldTimerManager().ClearTimer(StuckCheckTimer);

	if (bIsChasing)	return;

	if (bRecoveringFromStuck)
	{
		bRecoveringFromStuck = false;
		return;
	}

	// Aspetta prima di scegliere il prossimo punto
	GetWorldTimerManager().SetTimer(PatrolTimer, this, &ASpiderAIController::MoveToRandomPoint, PatrolWaitTime, false);
}


void ASpiderAIController::StartStuckCheck()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	// Evita di avere più timer contemporaneamente
	GetWorldTimerManager().ClearTimer(StuckCheckTimer);

	LastCheckedLocation = ControlledPawn->GetActorLocation();

	CurrentStuckTime = 0.0f;

	GetWorldTimerManager().SetTimer(StuckCheckTimer, this, &ASpiderAIController::CheckIfStuck, 0.1f, true);
}


void ASpiderAIController::CheckIfStuck()
{
	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn)
	{
		GetWorldTimerManager().ClearTimer(StuckCheckTimer);
		return;
	}

	const FVector CurrentLocation = ControlledPawn->GetActorLocation();

	const float DistanceMoved = FVector::Dist2D(CurrentLocation, LastCheckedLocation);
	if (DistanceMoved < StuckDistanceThreshold)
	{
		CurrentStuckTime += 0.1f;
	}
	else
	{
		CurrentStuckTime = 0.0f;
	}

	LastCheckedLocation = CurrentLocation;
	if (CurrentStuckTime >= MaxStuckTime)
	{
		// Ricalcola percorso
		GetWorldTimerManager().ClearTimer(StuckCheckTimer);

		CurrentStuckTime = 0.0f;

		bRecoveringFromStuck = true;

		StopMovement();

		MoveToRandomPoint();
	}
}


void ASpiderAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		CurrentTarget = Actor;
		bIsChasing = true;

		// Ferma eventuali timer del Patrol
		GetWorldTimerManager().ClearTimer(PatrolTimer);
		GetWorldTimerManager().ClearTimer(StuckCheckTimer);

		StopMovement();

		GetWorldTimerManager().SetTimer(ChaseTimer, this, &ASpiderAIController::UpdateChase, 0.2f, true);

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Green, TEXT("PLAYER VISTO - CHASE"));
		}
	}
	else
	{
		// Player perso
		CurrentTarget = nullptr;
		bIsChasing = false;

		GetWorldTimerManager().ClearTimer(ChaseTimer);

		StopMovement();

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(-1, 2.0f, FColor::Red, TEXT("PLAYER PERSO - PATROL"));
		}

		MoveToRandomPoint();
	}
}


void ASpiderAIController::UpdateChase()
{
	if (!CurrentTarget) return;

	APawn* ControlledPawn = GetPawn();
	if (!ControlledPawn) return;

	float Distance = FVector::Dist2D(ControlledPawn->GetActorLocation(), CurrentTarget->GetActorLocation());

	if (Distance > AttackRange)
	{
		MoveToActor(CurrentTarget, MoveAcceptanceRadius, false);
	}
	else
	{
		StopMovement();

		ASpiderEnemyPawn* Spider = Cast<ASpiderEnemyPawn>(ControlledPawn);
		if (Spider)
		{
			Spider->StartAttack();
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(1, 0.25f, FColor::Red, TEXT("ATTACK RANGE"));
		}
	}
}


