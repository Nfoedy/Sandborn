#include "CollectableKey.h"
#include "SandbornCharacter.h"


ACollectableKey::ACollectableKey()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	// Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetCollisionProfileName(TEXT("OverlapAllDynamic"));

	// Collection Capsule
	Capsule = CreateDefaultSubobject<UCapsuleComponent>(TEXT("Capsule"));
	Capsule->SetupAttachment(RootComp);
	Capsule->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	Capsule->SetCapsuleHalfHeight(150.0f);
	Capsule->SetCapsuleRadius(100.0f);

	// Audio
	CollectAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectAudio"));
	CollectAudio->SetupAttachment(RootComp);
	CollectAudio->SetAutoActivate(false);

	// Settings
	RotationSpeed = 100.0f;
	KeyIndex = 1;

	// State
	IsCollected = false;
}



void ACollectableKey::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsCollected) return;

	// Rotate Key
	Mesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

	// Check Player overlap
	TArray<AActor*> OverlappingActors;

	Capsule->GetOverlappingActors(OverlappingActors, ASandbornCharacter::StaticClass());

	if (OverlappingActors.IsEmpty()) return;

	ASandbornCharacter* Player = Cast<ASandbornCharacter>(OverlappingActors[0]);
	if (!Player) return;

	Player->AddKey(KeyIndex, Mesh->GetStaticMesh(),	GetClass());

	Collect();
}



void ACollectableKey::Collect()
{
	if (IsCollected) return;

	IsCollected = true;

	// Notify 
	OnCollected.Broadcast();

	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Audio
	CollectAudio->Play();
}