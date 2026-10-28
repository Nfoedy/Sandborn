#include "AutoTransporter.h"


UAutoTransporter::UAutoTransporter()
{
	PrimaryComponentTick.bCanEverTick = true;

	MoveTime = 2.0f;

	ArePointsSet = false;
	StartPoint = FVector(0, 0, 0);
	EndPoint = FVector::Zero();

	MovingToEnd = true;
}



void UAutoTransporter::BeginPlay()
{
	Super::BeginPlay();

}



void UAutoTransporter::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);


	// CurrentLocation = posizione attuale dell'Owner
	// TargetLocation = destinazione
	// Quando la raggiunge, MovingToEnd = false, allora riparte verso l'altro punto
	AActor* MyOwner = GetOwner();
	if (MyOwner && ArePointsSet)
	{
		FVector CurrentLocation = MyOwner->GetActorLocation();
		float Speed = FVector::Distance(StartPoint, EndPoint) / MoveTime;

		FVector TargetLocation = MovingToEnd ? EndPoint : StartPoint;
		if (!CurrentLocation.Equals(TargetLocation))
		{
			FVector NewLocation = FMath::VInterpConstantTo(CurrentLocation, TargetLocation, DeltaTime, Speed);
			MyOwner->SetActorLocation(NewLocation);
		}
		else
		{
			MovingToEnd = !MovingToEnd;
		}

	}

}


void UAutoTransporter::SetPoints(FVector Point1, FVector Point2)
{
	if (Point1.Equals(Point2)) return;

	StartPoint = Point1;
	EndPoint = Point2;

	ArePointsSet = true;

}

