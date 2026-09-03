#include "PressurePlate.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"


APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = true;

	Activated = false;

	//
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	//
	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(RootComp);
	TriggerMesh->SetIsReplicated(true);

	//
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetIsReplicated(true);
}


void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	TriggerMesh->SetVisibility(false);
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));



	
}


void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> OverlappingActors;
	AActor* TriggerActor = 0;
	TriggerMesh->GetOverlappingActors(OverlappingActors);

	for (int ActorIdx = 0; ActorIdx < OverlappingActors.Num(); ActorIdx++)
	{
		AActor* A = OverlappingActors[ActorIdx];
		if (A->ActorHasTag("TriggerActor"))
		{
			TriggerActor = A;
			break;
		}
		
		FString Msg = FString::Printf(TEXT("Name: %s"), *A->GetName());
		GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, Msg);
	}

	// Se esiste
	if (TriggerActor)
	{
		if (!Activated)
		{
			Activated = true;
			OnActivated.Broadcast();
		}
	}
	else
	{
		if (Activated)
		{
			Activated = false;
			OnDeactivated.Broadcast();
		}
	}
}

