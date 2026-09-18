#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/AudioComponent.h"
#include "CollectableKey.generated.h"


// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FCollectableKeyOnCollected);


UCLASS()
class SANDBORN_API ACollectableKey : public AActor
{
	GENERATED_BODY()


public:

	ACollectableKey();

	virtual void Tick(float DeltaTime) override;

	// Delegate
	UPROPERTY(BlueprintAssignable)
	FCollectableKeyOnCollected OnCollected;


protected:

	void Collect();


public:

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float RotationSpeed;

	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite)
	int32 KeyIndex;

	// State
	UPROPERTY(BlueprintReadOnly, VisibleAnywhere)
	bool IsCollected;


private:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCapsuleComponent* Capsule;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UAudioComponent* CollectAudio;
};