#include "BossSpawner.h"

#include "PressurePlate.h"

#include "Components/SceneComponent.h"
#include "GameFramework/Pawn.h"


ABossSpawner::ABossSpawner()
{
	PrimaryActorTick.bCanEverTick = false;

	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);
	bHasSpawned = false;
}


void ABossSpawner::BeginPlay()
{
	Super::BeginPlay();

	if (TriggerPressurePlate)
	{
		TriggerPressurePlate->OnActivated.AddDynamic(this, &ABossSpawner::SpawnBoss);
	}
}


void ABossSpawner::SpawnBoss()
{
	if (bHasSpawned) return;
	if (!BossClass) return;

	bHasSpawned = true;

	FActorSpawnParameters SpawnParams;

	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	GetWorld()->SpawnActor<APawn>(BossClass,GetActorLocation(), GetActorRotation(), SpawnParams);
}