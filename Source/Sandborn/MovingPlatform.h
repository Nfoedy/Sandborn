#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Transporter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "MovingPlatform.generated.h"

UCLASS()
class SANDBORN_API AMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AMovingPlatform();

protected:
	
	virtual void BeginPlay() override;

public:	

	// Scene component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	// Mesh component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// Transporter component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UTransporter* Transporter;

	// Arrow Point 1 
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UArrowComponent* Point1;

	// Arrow Point 2
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UArrowComponent* Point2;

	
	virtual void Tick(float DeltaTime) override;

};
