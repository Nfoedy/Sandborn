#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SceneComponent.h"
#include "CollectableKeyHolder.generated.h"


UCLASS()
class SANDBORN_API ACollectableKeyHolder : public AActor
{
	GENERATED_BODY()


public:

	ACollectableKeyHolder();

	virtual void Tick(float DeltaTime) override;

	void ActivateKey(UStaticMesh* NewKeyMesh);


protected:

	virtual void BeginPlay() override;


public:

	// Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite)
	float KeyMeshRotationSpeed;

	UPROPERTY(EditInstanceOnly, BlueprintReadWrite)
	int32 HolderIndex;


private:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* Mesh;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UStaticMeshComponent* KeyMesh;
};