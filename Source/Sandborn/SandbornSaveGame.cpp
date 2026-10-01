#include "SandbornSaveGame.h"


USandbornSaveGame::USandbornSaveGame()
{
	PlayerLocation = FVector::ZeroVector;
	PlayerRotation = FRotator::ZeroRotator;

	PlayerHealth = 3;
	PlayerCoins = 0;
}