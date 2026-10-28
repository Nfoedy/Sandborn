#include "SandbornGameMode.h"
#include "SandbornCharacter.h"
#include "SandbornSaveGame.h"
#include "TimerManager.h"
#include "SpiderEnemyPawn.h"
#include "SpiderBoss.h"
#include "BossSpawner.h"
#include "Kismet/GameplayStatics.h"


ASandbornGameMode::ASandbornGameMode()
{
	bGameFinished = false; 
	bShouldLoadSave = false;
}

void ASandbornGameMode::InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage)
{
	Super::InitGame(MapName, Options, ErrorMessage);

	bShouldLoadSave = Options.Contains(TEXT("LoadSave"));
}

void ASandbornGameMode::BeginPlay()
{
	Super::BeginPlay();

	if (bShouldLoadSave)
	{
		FTimerDelegate LoadDelegate;

		LoadDelegate.BindLambda([this](){LoadCurrentGame();});

		GetWorldTimerManager().SetTimerForNextTick(LoadDelegate);
	}
}


void ASandbornGameMode::Victory()
{
	if (bGameFinished) return;

	bGameFinished = true;

	OnVictory();

	UGameplayStatics::SetGamePaused(this, true);
}

// Save system
bool ASandbornGameMode::SaveCurrentGame()
{
	ASandbornCharacter* Player = Cast<ASandbornCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player) return false;

	USandbornSaveGame* SaveData = Cast<USandbornSaveGame>(UGameplayStatics::CreateSaveGameObject(USandbornSaveGame::StaticClass()));
	if (!SaveData) return false;

	SaveData->PlayerLocation = Player->GetActorLocation();
	SaveData->PlayerRotation = Player->GetActorRotation();
	SaveData->PlayerHealth = Player->GetHealth();
	SaveData->PlayerCoins = Player->GetCoins();
	SaveData->CollectedKeyIndices = Player->GetCollectedKeyIndices();

	// Spiders
	TArray<AActor*> FoundSpiders;

	UGameplayStatics::GetAllActorsOfClass(this, ASpiderEnemyPawn::StaticClass(), FoundSpiders);

	for (AActor* Actor : FoundSpiders)
	{
		ASpiderEnemyPawn* Spider = Cast<ASpiderEnemyPawn>(Actor);
		if (!Spider) continue;

		// SaveId 0 = non gestito da questo sistema
		if (Spider->GetSaveId() <= 0) continue;

		if (Spider->IsDeadForSave())
		{
			const int32 SpiderSaveId = Spider->GetSaveId();
			SaveData->DeadSpiderIds.Add(SpiderSaveId);
			SaveData->DeadSpiderTransforms.Add(SpiderSaveId, Spider->GetActorTransform());
		}
	}

	// Boss
	ASpiderBoss* Boss = Cast<ASpiderBoss>(UGameplayStatics::GetActorOfClass(this, ASpiderBoss::StaticClass()));

	if (Boss)
	{
		SaveData->bBossSpawned = true;
		SaveData->bBossDead = Boss->IsDeadForSave();
		SaveData->BossHealth = Boss->GetHealthForSave();
		SaveData->BossTransform = Boss->GetActorTransform();
	}
	else
	{
		SaveData->bBossSpawned = false;
		SaveData->bBossDead = false;
	}

	return UGameplayStatics::SaveGameToSlot(SaveData, SaveSlotName, 0);
}


// Load System
bool ASandbornGameMode::LoadCurrentGame()
{
	// Save
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0))
	{
		return false;
	}

	USandbornSaveGame* SaveData = Cast<USandbornSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));

	if (!SaveData)
	{
		return false;
	}

	// Player
	ASandbornCharacter* Player = Cast<ASandbornCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player)
	{
		return false;
	}

	// Player Transform
	Player->SetActorLocationAndRotation(SaveData->PlayerLocation, SaveData->PlayerRotation);

	// Player Stats
	Player->RestoreSavedStats(SaveData->PlayerHealth, SaveData->PlayerCoins);

	// Keys
	Player->RestoreCollectedKeys(SaveData->CollectedKeyIndices);

	// Spiders
	TArray<AActor*> FoundSpiders;

	UGameplayStatics::GetAllActorsOfClass(this, ASpiderEnemyPawn::StaticClass(), FoundSpiders);

	for (AActor* Actor : FoundSpiders)
	{ 
		ASpiderEnemyPawn* Spider = Cast<ASpiderEnemyPawn>(Actor);
		if (!Spider) continue;

		const int32 SpiderSaveId = Spider->GetSaveId();

		if (SpiderSaveId <= 0) continue;

		if (!SaveData->DeadSpiderIds.Contains(SpiderSaveId))
		{
			continue;
		}
	
		// Restore dead position
		if (const FTransform* SavedTransform = SaveData->DeadSpiderTransforms.Find(SpiderSaveId))
		{
			Spider->SetActorTransform(*SavedTransform);
		}

		// Check drop key
		const int32 DroppedKeyIndex = Spider->GetDroppedKeyIndex();

		const bool bKeyWasCollected = SaveData->CollectedKeyIndices.Contains(DroppedKeyIndex);

		// Restore spiders dead
		Spider->RestoreDeadState(bKeyWasCollected);
	}

	// Boss
	if (SaveData->bBossSpawned)
	{
		ABossSpawner* BossSpawner = Cast<ABossSpawner>(UGameplayStatics::GetActorOfClass(this, ABossSpawner::StaticClass()));

		if (BossSpawner)
		{
			APawn* SpawnedPawn = BossSpawner->SpawnBossFromSave();
			ASpiderBoss* Boss = Cast<ASpiderBoss>(SpawnedPawn);

			if (Boss)
			{
				Boss->SetActorTransform(SaveData->BossTransform);
				Boss->RestoreHealth(SaveData->BossHealth);

				if (SaveData->bBossDead)
				{
					const int32 DroppedKeyIndex = Boss->GetDroppedKeyIndex();
					const bool bKeyWasCollected = SaveData->CollectedKeyIndices.Contains(DroppedKeyIndex);

					Boss->RestoreDeadState(bKeyWasCollected);
				}
			}
		}
	}


	return true;
}


void ASandbornGameMode::GameOver()
{
	if (bGameFinished) return;

	bGameFinished = true;

	OnGameOver();

	UGameplayStatics::SetGamePaused(this, true);
}