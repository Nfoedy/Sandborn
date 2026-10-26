#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "BossSpawner.generated.h"


class APressurePlate;
class APawn;
class USceneComponent;


UCLASS()
class SANDBORN_API ABossSpawner : public AActor
{
	GENERATED_BODY()


public:

	ABossSpawner();

	APawn* SpawnBossFromSave();
	bool HasSpawned() const;


protected:

	virtual void BeginPlay() override;


private:

	// Scene Comp
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Components", meta = (AllowPrivateAccess = "true"))
	USceneComponent* RootComp;

	// Pressure Plate Trigger
	UPROPERTY(EditInstanceOnly, Category = "Boss Spawner")
	APressurePlate* TriggerPressurePlate;

	// BP Spawn
	UPROPERTY(EditAnywhere, Category = "Boss Spawner")
	TSubclassOf<APawn> BossClass;

	UPROPERTY(VisibleAnywhere, Category = "Boss Spawner")
	bool bHasSpawned;

	UFUNCTION()
	void SpawnBoss();

	APawn* SpawnBossInternal();
};