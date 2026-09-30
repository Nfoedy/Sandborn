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


protected:

	UFUNCTION(BlueprintImplementableEvent, Category = "Match")
	void OnVictory();


private:

	bool bGameFinished;
};