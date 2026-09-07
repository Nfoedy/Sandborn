#include "CollectableKey.h"
#include "SandbornCharacter.h"


ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;

	IsCollected = false;

	//
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	//
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetCollisionProfileName(FName("OverlapAllDynamic"));

	//
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComp);
	Capsule->SetCollisionProfileName(FName("OverlapAllDynamic"));
	Capsule->SetCapsuleHalfHeight(150.0f);
	Capsule->SetCapsuleRadius(100.0f);

	// 
	CollectAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectAudio"));
	CollectAudio->SetupAttachment(RootComp);
	CollectAudio->SetAutoActivate(false);

	//
	RotationSpeed = 100;
}


void ACollectableKey::BeginPlay()
{
	Super::BeginPlay();
}


void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the mesh
	Mesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

	TArray<AActor*> OverlapActors;

	Capsule->GetOverlappingActors(OverlapActors, ASandbornCharacter::StaticClass());

	if (OverlapActors.Num() > 0)
	{
		// Il player sta attraversando la capsula

		if (!IsCollected)
		{
			Collect();
		}
	}
}


void ACollectableKey::Collect()
{

	if (IsCollected) return;

	IsCollected = true;

	OnCollected.Broadcast();

	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollectAudio->Play();
}