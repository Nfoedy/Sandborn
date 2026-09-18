#include "CollectableKeyHolder.h"


ACollectableKeyHolder::ACollectableKeyHolder()
{
	PrimaryActorTick.bCanEverTick = true;

	// Root
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	// Holder Mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetCollisionProfileName(TEXT("BlockAllDynamic"));

	// Key Mesh
	KeyMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("KeyMesh"));
	KeyMesh->SetupAttachment(RootComp);
	KeyMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Settings
	KeyMeshRotationSpeed = 100.0f;
	HolderIndex = 1;
}


void ACollectableKeyHolder::BeginPlay()
{
	Super::BeginPlay();

	KeyMesh->SetVisibility(false);
}


void ACollectableKeyHolder::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (!KeyMesh->IsVisible()) return;

	KeyMesh->AddRelativeRotation(FRotator(0.0f, KeyMeshRotationSpeed * DeltaTime, 0.0f));
}


void ACollectableKeyHolder::ActivateKey(UStaticMesh* NewKeyMesh)
{
	if (!NewKeyMesh) return;

	KeyMesh->SetStaticMesh(NewKeyMesh);
	KeyMesh->SetVisibility(true);
}