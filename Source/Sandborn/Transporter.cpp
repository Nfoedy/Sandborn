#include "Transporter.h"

#include "CollectableKey.h"
#include "PressurePlate.h"
#include "SandbornCharacter.h"

#include "Kismet/GameplayStatics.h"


UTransporter::UTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;

	MoveTime = 3.0f;

	OwnerIsTriggerActor = false;

	ActivatedTriggerCount = 0;
	AllTriggerActorsTriggered = false;

	ArePointsSet = false;

	StartPoint = FVector::ZeroVector;
	EndPoint = FVector::ZeroVector;
}


void UTransporter::BeginPlay()
{
	Super::BeginPlay();

	if (OwnerIsTriggerActor)
	{
		TriggerActors.AddUnique(GetOwner());
	}

	for (AActor* TriggerActor : TriggerActors)
	{
		if (!TriggerActor) continue;

		if (APressurePlate* PressurePlate = Cast<APressurePlate>(TriggerActor))
		{
			PressurePlate->OnActivated.AddDynamic(this,	&UTransporter::OnTriggerActorActivated);
			PressurePlate->OnDeactivated.AddDynamic(this, &UTransporter::OnTriggerActorDeactivated);
			continue;
		}

		if (ACollectableKey* Key = Cast<ACollectableKey>(TriggerActor))
		{
			Key->OnCollected.AddDynamic(this, &UTransporter::OnTriggerActorActivated);
		}
	}

	SpawnedKeyTriggerStates.Init(false, SpawnedKeyTriggers.Num());

	ASandbornCharacter* Player = Cast<ASandbornCharacter>(UGameplayStatics::GetPlayerCharacter(GetWorld(), 0));
	if (Player)
	{
		Player->OnKeyCollected.AddDynamic(this,	&UTransporter::OnSpawnedKeyCollected);
	}
}


void UTransporter::SetPoints(FVector Point1, FVector Point2)
{
	if (Point1.Equals(Point2)) return;

	StartPoint = Point1;
	EndPoint = Point2;

	ArePointsSet = true;
}


void UTransporter::OnTriggerActorActivated()
{
	ActivatedTriggerCount++;
}


void UTransporter::OnTriggerActorDeactivated()
{
	ActivatedTriggerCount = FMath::Max(ActivatedTriggerCount - 1, 0);
}


void UTransporter::OnSpawnedKeyCollected(UClass* KeyClass)
{
	if (!KeyClass) return;

	for (int32 Index = 0; Index < SpawnedKeyTriggers.Num(); Index++)
	{
		if (SpawnedKeyTriggerStates[Index]) continue;
		if (!SpawnedKeyTriggers[Index]) continue;

		if (KeyClass->IsChildOf(SpawnedKeyTriggers[Index].Get()))
		{
			SpawnedKeyTriggerStates[Index] = true;

			OnTriggerActorActivated();

			break;
		}
	}
}


void UTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	const int32 RequiredTriggerCount = TriggerActors.Num() + SpawnedKeyTriggers.Num();

	AllTriggerActorsTriggered =	RequiredTriggerCount > 0 && ActivatedTriggerCount >= RequiredTriggerCount;

	AActor* Owner = GetOwner();
	if (!Owner || !ArePointsSet) return;

	const FVector CurrentLocation = Owner->GetActorLocation();

	const float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

	const FVector TargetLocation = AllTriggerActorsTriggered ? EndPoint : StartPoint;

	if (CurrentLocation.Equals(TargetLocation)) return;

	const FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
	
	Owner->SetActorLocation(NewLocation);
}