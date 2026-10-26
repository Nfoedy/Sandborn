#include "SpiderBoss.h"

#include "Engine/Engine.h"


ASpiderBoss::ASpiderBoss()
{
	MaxHealth = 3;
	Health = 3;
}


void ASpiderBoss::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}


void ASpiderBoss::HandleStomp()
{
	if (Health <= 0) return;

	Health--;


	if (GEngine)
	{
		GEngine->AddOnScreenDebugMessage(
			-1,
			2.0f,
			FColor::Red,
			FString::Printf(
				TEXT("BOSS HP: %d / %d"),
				Health,
				MaxHealth
			)
		);
	}


	if (Health <= 0)
	{
		Die();
	}
}


int32 ASpiderBoss::GetHealthForSave() const
{
	return Health;
}


void ASpiderBoss::RestoreHealth(int32 SavedHealth)
{
	Health = FMath::Clamp(SavedHealth, 0, MaxHealth);
}