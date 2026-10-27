#include "SpiderAIController.h"
#include "SandbornCharacter.h"
#include "Components/StateTreeAIComponent.h"
#include "Perception/AIPerceptionComponent.h"
#include "Perception/AISenseConfig_Sight.h"
#include "Engine/Engine.h"


ASpiderAIController::ASpiderAIController()
{
	// State Tree
	StateTreeComponent = CreateDefaultSubobject<UStateTreeAIComponent>(TEXT("StateTreeComponent"));
	BrainComponent = StateTreeComponent;

	// AI Perception
	AIPerception = CreateDefaultSubobject<UAIPerceptionComponent>(TEXT("AIPerception"));
	SetPerceptionComponent(*AIPerception);

	SightConfig = CreateDefaultSubobject<UAISenseConfig_Sight>(TEXT("SightConfig"));
	SightConfig->SightRadius = 1500.0f;
	SightConfig->LoseSightRadius = 1800.0f;
	SightConfig->PeripheralVisionAngleDegrees = 70.0f;
	SightConfig->DetectionByAffiliation.bDetectEnemies = true;
	SightConfig->DetectionByAffiliation.bDetectFriendlies = true;
	SightConfig->DetectionByAffiliation.bDetectNeutrals = true;

	AIPerception->ConfigureSense(*SightConfig);
	AIPerception->SetDominantSense(SightConfig->GetSenseImplementation());

	// Perception Data
	CurrentTarget = nullptr;
}


void ASpiderAIController::BeginPlay()
{
	Super::BeginPlay();

	AIPerception->OnTargetPerceptionUpdated.AddDynamic(this, &ASpiderAIController::OnTargetPerceptionUpdated);

	if (StateTreeComponent)
	{
		StateTreeComponent->StartLogic();
	}
}


void ASpiderAIController::OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus)
{
	if (!Actor) return;

	// Il ragno può avere come target solamente il Player
	ASandbornCharacter* Player = Cast<ASandbornCharacter>(Actor);
	if (!Player) return;

	if (Stimulus.WasSuccessfullySensed())
	{
		CurrentTarget = Player;

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Green,
				TEXT("PLAYER VISTO")
			);
		}
	}
	else
	{
		if (CurrentTarget == Player)
		{
			CurrentTarget = nullptr;
		}

		if (GEngine)
		{
			GEngine->AddOnScreenDebugMessage(
				-1,
				2.0f,
				FColor::Red,
				TEXT("PLAYER PERSO")
			);
		}
	}
}