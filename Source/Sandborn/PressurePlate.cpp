#include "PressurePlate.h"
#include "Components/StaticMeshComponent.h"


APressurePlate::APressurePlate()
{
	PrimaryActorTick.bCanEverTick = true;

	Activated = false;

	// creo il root comp
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	// creo il trigger mesh ed imposto il rispettivo scale
	TriggerMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TriggerMesh"));
	TriggerMesh->SetupAttachment(RootComp);

	// Creo la mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);

	// Creao i trasport component
	Transporter = CreateDefaultSubobject<UTransporter>(TEXT("Transporter"));


}


void APressurePlate::BeginPlay()
{
	Super::BeginPlay();

	TriggerMesh->SetVisibility(false);
	TriggerMesh->SetCollisionProfileName(FName("OverlapAll"));

	// Pressure Plate transport aniamtion
	FVector Point1 = GetActorLocation();
	FVector Point2 = Point1 + FVector(0.0f, 0.0f, -10.0f);
	Transporter->SetPoints(Point1, Point2);
	
	

}


void APressurePlate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TArray<AActor*> OverlappingActors;
	AActor* TriggerActor = nullptr;

	TriggerMesh->GetOverlappingActors(OverlappingActors);

	for (int ActorIdx = 0; ActorIdx < OverlappingActors.Num(); ActorIdx++)
	{
		AActor* A = OverlappingActors[ActorIdx];

		if (A->ActorHasTag("TriggerActor"))
		{
			TriggerActor = A;
			break;
		}

		//FString Msg = FString::Printf(TEXT("Name: %s"), *A->GetName());
		//GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, Msg);
	}

	// Se esiste
	if (TriggerActor)
	{
		if (!Activated)
		{
			Activated = true;
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Attivata");
			OnActivated.Broadcast();
		}
	}
	else
	{
		if (Activated)
		{
			Activated = false;
			GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::White, "Disattivata");
			OnDeactivated.Broadcast();
		}
	}
}