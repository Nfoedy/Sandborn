#include "SpiderBoss.h"

#include "Engine/Engine.h"


ASpiderBoss::ASpiderBoss()
{
	MaxHealth = 5;
	Health = 5;
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