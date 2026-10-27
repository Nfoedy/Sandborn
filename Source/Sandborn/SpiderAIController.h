#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "Perception/AIPerceptionTypes.h"
#include "SpiderAIController.generated.h"


class UAIPerceptionComponent;
class UAISenseConfig_Sight;
class UStateTreeAIComponent;


UCLASS()
class SANDBORN_API ASpiderAIController : public AAIController
{
	GENERATED_BODY()

public:

	ASpiderAIController();

	// Perception Data
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|Perception")
	TObjectPtr<AActor> CurrentTarget;


protected:

	virtual void BeginPlay() override;

	virtual void OnPossess(APawn* InPawn) override;
	virtual void OnUnPossess() override;

private:

	// StateTree
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "AI|StateTree", meta = (AllowPrivateAccess = "true"))
	TObjectPtr<UStateTreeAIComponent> StateTreeComponent;

	// AI Perception
	UPROPERTY(VisibleAnywhere, Category = "AI|Perception")
	TObjectPtr<UAIPerceptionComponent> AIPerception;

	UPROPERTY(VisibleAnywhere, Category = "AI|Perception")
	TObjectPtr<UAISenseConfig_Sight> SightConfig;


	UFUNCTION()
	void OnTargetPerceptionUpdated(AActor* Actor, FAIStimulus Stimulus);

};