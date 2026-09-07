#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "CollectableKey.generated.h"


// DELEGATES
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCollectableKeyOnCollected);


UCLASS()
class SANDBORN_API ACollectableKey : public AActor
{
	GENERATED_BODY()

public:

	ACollectableKey();

protected:

	virtual void BeginPlay() override;

public:

	virtual void Tick(float DeltaTime) override;

	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float RotationSpeed;

	// Scene component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	// Capsule component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UCapsuleComponent* Capsule;

	// Static Mesh component 
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool IsCollected;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UAudioComponent* CollectAudio;

	void Collect();

	// Delegates
	UPROPERTY(BlueprintAssignable)
	FCollectableKeyOnCollected OnCollected;
};