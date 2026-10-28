#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Transporter.generated.h"

class ACollectableKey;
class UStaticMesh;


UCLASS(ClassGroup = (Custom), meta = (BlueprintSpawnableComponent))
class SANDBORN_API UTransporter : public UActorComponent
{
	GENERATED_BODY()

public:

	UTransporter();

protected:

	virtual void BeginPlay() override;

public:

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	FVector StartPoint;
	FVector EndPoint;

	bool ArePointsSet;

	TArray<bool> SpawnedKeyTriggerStates;

	UPROPERTY(EditAnywhere)
	float MoveTime;

	UPROPERTY(EditAnywhere)
	bool OwnerIsTriggerActor;

	UPROPERTY(EditAnywhere)
	TArray<AActor*> TriggerActors;

	UPROPERTY(VisibleAnywhere)
	int ActivatedTriggerCount;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	bool AllTriggerActorsTriggered;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<ACollectableKey>> SpawnedKeyTriggers;

	UFUNCTION(BlueprintCallable)
	void SetPoints(FVector Point1, FVector Point2);

	UFUNCTION()
	void OnTriggerActorActivated();

	UFUNCTION()
	void OnTriggerActorDeactivated();

	UFUNCTION()
	void OnSpawnedKeyCollected(UClass* KeyClass);

	void RestoreCollectedKeys(const TArray<int32>& SavedKeyIndices);

	UStaticMesh* FindKeyMeshByIndex(int32 KeyIndex) const;
};