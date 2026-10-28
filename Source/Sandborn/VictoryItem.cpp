#include "VictoryItem.h"
#include "SandbornCharacter.h"
#include "Components/SceneComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"
#include "SandbornGameMode.h"
#include "Kismet/GameplayStatics.h"


AVictoryItem::AVictoryItem()
{
	PrimaryActorTick.bCanEverTick = true;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);
	Mesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	Collision = CreateDefaultSubobject<USphereComponent>(TEXT("Collision"));
	Collision->SetupAttachment(RootComp);
	Collision->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	Collision->SetCollisionResponseToAllChannels(ECR_Ignore);
	Collision->SetCollisionResponseToChannel(ECC_Pawn, ECR_Overlap);
	Collision->SetSphereRadius(100.0f);

	RotationSpeed = 100.0f;
	bCollected = false;


	Collision->OnComponentBeginOverlap.AddDynamic(this, &AVictoryItem::OnOverlap);
}


void AVictoryItem::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (bCollected) return;

	Mesh->AddLocalRotation(FRotator(0.0f, RotationSpeed * DeltaTime, 0.0f));
}


void AVictoryItem::OnOverlap(
	UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor,
	UPrimitiveComponent* OtherComp,
	int32 OtherBodyIndex,
	bool bFromSweep,
	const FHitResult& SweepResult
)
{
	ASandbornCharacter* Player = Cast<ASandbornCharacter>(OtherActor);
	if (!Player) return;

	Collect();
}


void AVictoryItem::Collect()
{
	if (bCollected) return;

	bCollected = true;

	Mesh->SetVisibility(false);
	Collision->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	ASandbornGameMode* GameMode = Cast<ASandbornGameMode>(UGameplayStatics::GetGameMode(this));
	if (GameMode)
	{
		GameMode->Victory();
	}
}