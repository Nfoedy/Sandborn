#include "AutoMovingPlatform.h"


AAutoMovingPlatform::AAutoMovingPlatform()
{
	PrimaryActorTick.bCanEverTick = true;

	// creo il root comp
	RootComp = CreateDefaultSubobject<USceneComponent>(TEXT("RootComp"));
	SetRootComponent(RootComp);

	// Creo la mesh
	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComp);

	// Creo i due points 
	Point1 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point1"));
	Point1->SetupAttachment(RootComp);
	Point1->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	Point2 = CreateDefaultSubobject<UArrowComponent>(TEXT("Point2"));
	Point2->SetupAttachment(RootComp);
	Point2->SetRelativeLocation(FVector(0.0f, 0.0f, 300.0f));

	// Creao i trasport component
	AutoTransporter = CreateDefaultSubobject<UAutoTransporter>(TEXT("AutoTransporter"));

}

void AAutoMovingPlatform::BeginPlay()
{
	Super::BeginPlay();

	// Pressure Plate transport aniamtion
	FVector StartPoint = GetActorLocation() + Point1->GetRelativeLocation();
	FVector EndPoint = GetActorLocation() + Point2->GetRelativeLocation();
	AutoTransporter->SetPoints(StartPoint, EndPoint);
	
}


void AAutoMovingPlatform::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

