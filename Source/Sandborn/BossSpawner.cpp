#include "BossSpawner.h"
#include "PressurePlate.h"
#include "SpiderBoss.h"
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
	SpawnBossInternal();
}


APawn* ABossSpawner::SpawnBossFromSave()
{
	return SpawnBossInternal();
}


bool ABossSpawner::HasSpawned() const
{
	return bHasSpawned;
}


APawn* ABossSpawner::SpawnBossInternal()
{
	if (bHasSpawned) return nullptr;
	if (!BossClass) return nullptr;

	FActorSpawnParameters SpawnParams;
	SpawnParams.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;

	APawn* SpawnedBoss = GetWorld()->SpawnActor<APawn>(BossClass, GetActorLocation(), GetActorRotation(), SpawnParams);

	if (SpawnedBoss)
	{
		bHasSpawned = true;

		UE_LOG(
			LogTemp,
			Warning,
			TEXT("[BOSS DEBUG] Boss: %s | Controller: %s"),
			*GetNameSafe(SpawnedBoss),
			*GetNameSafe(SpawnedBoss->GetController())
		);


		if (ASpiderBoss* Boss = Cast<ASpiderBoss>(SpawnedBoss))
		{
			OnBossSpawned.Broadcast(Boss);
		}
	}

	return SpawnedBoss;
}