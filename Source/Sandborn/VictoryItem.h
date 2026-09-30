#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "VictoryItem.generated.h"


class USceneComponent;
class UStaticMeshComponent;
class USphereComponent;


UCLASS()
class SANDBORN_API AVictoryItem : public AActor
{
	GENERATED_BODY()


public:

	AVictoryItem();

	virtual void Tick(float DeltaTime) override;


private:

	// Scene Comp
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USceneComponent* RootComp;

	// Mesh
	UPROPERTY(VisibleAnywhere, Category = "Components")
	UStaticMeshComponent* Mesh;

	// Sphere Comp
	UPROPERTY(VisibleAnywhere, Category = "Components")
	USphereComponent* Collision;

	// Rotation Speed
	UPROPERTY(EditAnywhere, Category = "Victory Item")
	float RotationSpeed;

	bool bCollected;

	UFUNCTION()
	void OnOverlap(
		UPrimitiveComponent* OverlappedComponent,
		AActor* OtherActor,
		UPrimitiveComponent* OtherComp,
		int32 OtherBodyIndex,
		bool bFromSweep,
		const FHitResult& SweepResult
	);

	void Collect();
};