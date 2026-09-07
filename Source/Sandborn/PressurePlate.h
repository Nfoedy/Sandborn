#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Transporter.h"
#include "Components/StaticMeshComponent.h"
#include "PressurePlate.generated.h"


// DELEGATES
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPressurePlateOnActivated);
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FPressurePlateOnDeactivated);



UCLASS()
class SANDBORN_API APressurePlate : public AActor
{
	GENERATED_BODY()
	
public:	
	
	APressurePlate();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	// Scene component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	// Trigger Mesh
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* TriggerMesh;

	// Mesh
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// Transporter component 
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UTransporter* Transporter;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool Activated;

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FPressurePlateOnActivated OnActivated;

	UPROPERTY(BlueprintAssignable)
	FPressurePlateOnDeactivated OnDeactivated;

	

};
