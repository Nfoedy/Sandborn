#include "CollectableCoin.h"
#include "SandbornCharacter.h"


ACollectableCoin::ACollectableCoin()
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
	Capsule->SetCapsuleHalfHeight(100.0f);
	Capsule->SetCapsuleRadius(50.0f);

	// 
	CollectAudio = CreateDefaultSubobject<UAudioComponent>(TEXT("CollectAudio"));
	CollectAudio->SetupAttachment(RootComp);
	CollectAudio->SetAutoActivate(false);

	RotationSpeed = 100.0f;

}


void ACollectableCoin::BeginPlay()
{
	Super::BeginPlay();
	
}


void ACollectableCoin::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Rotate the mesh
	Mesh->AddRelativeRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));

	TArray<AActor*> OverlapActors;

	Capsule->GetOverlappingActors(OverlapActors, ASandbornCharacter::StaticClass());

	if (OverlapActors.Num() > 0)
	{

		AActor* FirstActor = OverlapActors[0];
		ASandbornCharacter* Character = Cast<ASandbornCharacter>(FirstActor);

		if (Character)
		{
			if (!IsCollected)
			{
				Collect(Character);
			}
		}

	}

}


void ACollectableCoin::Collect(ASandbornCharacter* Character)
{
	if (IsCollected) return;
	if (!Character) return;
	if (Character->IsDead) return;

	IsCollected = true;

	Character->AddCoin();

	Mesh->SetVisibility(false);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	Capsule->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	CollectAudio->Play();

}
