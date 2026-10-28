#include "SpiderBoss.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "Engine/Engine.h"


ASpiderBoss::ASpiderBoss()
{
	MaxHealth = 3;
	Health = 3;

	ChargeSpeedMultiplier = 1.8f;
	SpeedBeforeCharge = 0.0f;
	bIsCharging = false;

	EnragedSpeedMultiplier = 1.3f;
	SpeedBeforeEnraged = 0.0f;
	bIsEnraged = false;

	StompDamageCooldown = 0.5f;
	bCanTakeStompDamage = true;

}


void ASpiderBoss::BeginPlay()
{
	Super::BeginPlay();

	Health = MaxHealth;
}


void ASpiderBoss::HandleStomp()
{
	if (Health <= 0) return;
	if (!bCanTakeStompDamage) return;

	bCanTakeStompDamage = false;

	GetWorldTimerManager().SetTimer(
		StompDamageCooldownTimer,
		this,
		&ASpiderBoss::ResetStompDamage,
		StompDamageCooldown,
		false
	);

	Health--;

	OnBossHealthChanged.Broadcast(Health, MaxHealth);

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

	OnBossHealthChanged.Broadcast(Health,MaxHealth);
}


int32 ASpiderBoss::GetCurrentHealth() const
{
	return Health;
}

int32 ASpiderBoss::GetMaxHealth() const
{
	return MaxHealth;
}


void ASpiderBoss::StartCharge()
{
	if (bIsCharging) return;

	UFloatingPawnMovement* Movement = Cast<UFloatingPawnMovement>(GetMovementComponent());

	if (!Movement) return;

	SpeedBeforeCharge = Movement->MaxSpeed;
	 
	Movement->MaxSpeed = SpeedBeforeCharge * ChargeSpeedMultiplier;

	bIsCharging = true;
}


void ASpiderBoss::EndCharge()
{
	if (!bIsCharging) return;

	UFloatingPawnMovement* Movement = Cast<UFloatingPawnMovement>(GetMovementComponent());

	if (Movement && SpeedBeforeCharge > 0.0f)
	{
		Movement->MaxSpeed = SpeedBeforeCharge;
	}

	bIsCharging = false;
}


void ASpiderBoss::StartEnraged()
{
	if (bIsEnraged) return;

	UFloatingPawnMovement* Movement = Cast<UFloatingPawnMovement>(GetMovementComponent());

	if (!Movement) return;

	SpeedBeforeEnraged = Movement->MaxSpeed;

	Movement->MaxSpeed = SpeedBeforeEnraged * EnragedSpeedMultiplier;

	bIsEnraged = true;
}


void ASpiderBoss::EndEnraged()
{
	if (!bIsEnraged) return;

	UFloatingPawnMovement* Movement = Cast<UFloatingPawnMovement>(GetMovementComponent());

	if (Movement && SpeedBeforeEnraged > 0.0f)
	{
		Movement->MaxSpeed = SpeedBeforeEnraged;
	}

	bIsEnraged = false;
}





void ASpiderBoss::ResetStompDamage()
{
	bCanTakeStompDamage = true;
}