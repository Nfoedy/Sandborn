#pragma once

#include "CoreMinimal.h"
#include "GameFramework/SaveGame.h"
#include "SandbornSaveGame.generated.h"


UCLASS()
class SANDBORN_API USandbornSaveGame : public USaveGame
{
	GENERATED_BODY()


public:

	USandbornSaveGame();

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|Player")
	FVector PlayerLocation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|Player")
	FRotator PlayerRotation;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|Player")
	int32 PlayerHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|Player")
	int32 PlayerCoins;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|Player")
	TArray<int32> CollectedKeyIndices;

	// Spiders
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|World")
	TArray<int32> DeadSpiderIds;
	 
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|World")
	TMap<int32, FTransform> DeadSpiderTransforms;


	// Boss
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|Boss")
	bool bBossSpawned;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|Boss")
	bool bBossDead;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|Boss")
	int32 BossHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, SaveGame, Category = "Save|Boss")
	FTransform BossTransform;
};