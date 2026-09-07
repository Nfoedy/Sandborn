#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "CollectableKey.h"
#include "CollectableKeyHolder.generated.h"

UCLASS()
class SANDBORN_API ACollectableKeyHolder : public AActor
{
	GENERATED_BODY()
	
public:	
	
	ACollectableKeyHolder();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	// Key mesh rotation speed
	UPROPERTY(BlueprintReadWrite, EditAnywhere)
	float KeyMeshRotationSpeed;

	// Root Component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	USceneComponent* RootComp;

	// Mesh component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* Mesh;

	// Key mesh component
	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	UStaticMeshComponent* KeyMesh;

	// KeyRef
	UPROPERTY(EditInstanceOnly)
	ACollectableKey* KeyRef;

	UFUNCTION()
	void ActivateKeyMesh();
};
