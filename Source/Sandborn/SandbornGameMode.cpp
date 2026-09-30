#include "SandbornGameMode.h"
#include "Kismet/GameplayStatics.h"


ASandbornGameMode::ASandbornGameMode()
{
	bGameFinished = false;
}


void ASandbornGameMode::Victory()
{
	if (bGameFinished) return;

	bGameFinished = true;

	OnVictory();

	UGameplayStatics::SetGamePaused(this, true);
}