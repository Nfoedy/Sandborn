#include "SandbornGameMode.h"
#include "SandbornCharacter.h"
#include "SandbornSaveGame.h"
#include "TimerManager.h"
#include "SpiderEnemyPawn.h"
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

	return UGameplayStatics::SaveGameToSlot(SaveData, SaveSlotName, 0);
}


// Load System
bool ASandbornGameMode::LoadCurrentGame()
{
	if (!UGameplayStatics::DoesSaveGameExist(SaveSlotName, 0)) return false;

	USandbornSaveGame* SaveData = Cast<USandbornSaveGame>(UGameplayStatics::LoadGameFromSlot(SaveSlotName, 0));
	if (!SaveData) return false;

	ASandbornCharacter* Player = Cast<ASandbornCharacter>(UGameplayStatics::GetPlayerCharacter(this, 0));
	if (!Player) return false;

	// Transform
	Player->SetActorLocationAndRotation(SaveData->PlayerLocation, SaveData->PlayerRotation);

	// Player State
	Player->RestoreSavedStats(SaveData->PlayerHealth, SaveData->PlayerCoins);

	// Player Keys
	Player->RestoreCollectedKeys(SaveData->CollectedKeyIndices);

	// Spiders
	TArray<AActor*> FoundSpiders;

	UGameplayStatics::GetAllActorsOfClass(this, ASpiderEnemyPawn::StaticClass(), FoundSpiders);

	for (AActor* Actor : FoundSpiders)
	{
		ASpiderEnemyPawn* Spider = Cast<ASpiderEnemyPawn>(Actor);
		if (!Spider) continue;

		if (Spider->GetSaveId() <= 0) continue;

		const int32 SpiderSaveId = Spider->GetSaveId();

		if (SaveData->DeadSpiderIds.Contains(SpiderSaveId))
		{
			if (const FTransform* SavedTransform = SaveData->DeadSpiderTransforms.Find(SpiderSaveId))
			{
				Spider->SetActorTransform(*SavedTransform);
			}

			Spider->RestoreDeadState();
		}
	}


	return true;
}