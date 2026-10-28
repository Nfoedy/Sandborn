#pragma once

#include "CoreMinimal.h"
#include "SpiderEnemyPawn.h"
#include "SpiderBoss.generated.h"


DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FBossHealthChangedEvent, int32, CurrentHealth, int32, MaxHealth);

UCLASS()
class SANDBORN_API ASpiderBoss : public ASpiderEnemyPawn
{
	GENERATED_BODY()


public:

	ASpiderBoss();

	virtual void HandleStomp() override;

	int32 GetHealthForSave() const;
	void RestoreHealth(int32 SavedHealth);

	UPROPERTY(BlueprintAssignable, Category = "Boss|Health")
	FBossHealthChangedEvent OnBossHealthChanged;

	UFUNCTION(BlueprintPure, Category = "Boss|Health")
	int32 GetCurrentHealth() const;

	UFUNCTION(BlueprintPure, Category = "Boss|Health")
	int32 GetMaxHealth() const;

	UFUNCTION(BlueprintCallable, Category = "Boss|Charge")
	void StartCharge();

	UFUNCTION(BlueprintCallable, Category = "Boss|Charge")
	void EndCharge();

	UFUNCTION(BlueprintCallable, Category = "Boss|Enraged")
	void StartEnraged();

	UFUNCTION(BlueprintCallable, Category = "Boss|Enraged")
	void EndEnraged();


protected:

	virtual void BeginPlay() override;


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Health", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Health", meta = (AllowPrivateAccess = "true"))
	int32 Health;

	// Charge
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Charge", meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
	float ChargeSpeedMultiplier;

	float SpeedBeforeCharge;

	bool bIsCharging;

	// Enraged
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Enraged", meta = (AllowPrivateAccess = "true", ClampMin = "1.0"))
	float EnragedSpeedMultiplier;

	float SpeedBeforeEnraged;

	bool bIsEnraged;

	// Stomp Damage
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Health", meta = (AllowPrivateAccess = "true", ClampMin = "0.0"))
	float StompDamageCooldown;

	bool bCanTakeStompDamage;

	FTimerHandle StompDamageCooldownTimer;

	void ResetStompDamage();
};