#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "SandbornGameMode.generated.h"


UCLASS(abstract)
class ASandbornGameMode : public AGameModeBase
{
	GENERATED_BODY()


public:

	ASandbornGameMode();

	void Victory();

	// Save System
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool SaveCurrentGame();

	// Load System
	UFUNCTION(BlueprintCallable, Category = "Save")
	bool LoadCurrentGame();


protected:

	virtual void InitGame(const FString& MapName, const FString& Options, FString& ErrorMessage) override;
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintImplementableEvent, Category = "Match")
	void OnVictory();


private:

	bool bGameFinished;

	bool bShouldLoadSave;

	// Save Settings
	UPROPERTY(EditDefaultsOnly, Category = "Save")
	FString SaveSlotName = TEXT("SandbornSave");
};