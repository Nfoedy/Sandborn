#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "TimerManager.h"
#include "SandbornCharacter.generated.h"


class USpringArmComponent;
class UCameraComponent;
class UInputAction;
class UStaticMesh;
class ACollectableKeyHolder;

struct FInputActionValue;


// Delegates
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FKeyCollectedEvent, UClass*, KeyClass);


UCLASS(abstract)
class SANDBORN_API ASandbornCharacter : public ACharacter
{
	GENERATED_BODY()


public:

	ASandbornCharacter();

	virtual float TakeDamage(float DamageAmount, const FDamageEvent& DamageEvent, AController* EventInstigator, AActor* DamageCauser) override;

	// Gameplay
	void AddCoin();

	void AddKey(int32 KeyIndex, UStaticMesh* KeyMesh, UClass* KeyClass);

	void LoseHealth();

	void Die();

	
	// Input
	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoMove(float Right, float Forward);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoLook(float Yaw, float Pitch);

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpStart();

	UFUNCTION(BlueprintCallable, Category = "Input")
	virtual void DoJumpEnd();


	// Event
	UPROPERTY(BlueprintAssignable, Category = "Player|Keys")
	FKeyCollectedEvent OnKeyCollected;

	// Death
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	bool IsDead;


	// Getters	
	FORCEINLINE USpringArmComponent* GetCameraBoom() const
	{
		return CameraBoom;
	}

	FORCEINLINE UCameraComponent* GetFollowCamera() const
	{
		return FollowCamera;
	}

	int32 GetHealth() const;
	int32 GetCoins() const;
	const TArray<int32>& GetCollectedKeyIndices() const;

	void RestoreSavedStats(int32 SavedHealth, int32 SavedCoins);
	void RestoreCollectedKeys(const TArray<int32>& SavedKeyIndices);


protected:

	virtual void BeginPlay() override;

	virtual void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent) override;


private:

	// Components
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	USpringArmComponent* CameraBoom;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FollowCamera;


	// inputs
	UPROPERTY(EditAnywhere)
	UInputAction* JumpAction;

	UPROPERTY(EditAnywhere)
	UInputAction* MoveAction;

	UPROPERTY(EditAnywhere)
	UInputAction* LookAction;

	UPROPERTY(EditAnywhere)
	UInputAction* MouseLookAction;

	UPROPERTY(EditAnywhere)
	UInputAction* DebugDamageAction;


	void Move(const FInputActionValue& Value);

	void Look(const FInputActionValue& Value);


	// Coins
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CollectedCoins;


	// Keys
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 CollectedKeys;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxKeys;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	TArray<int32> CollectedKeyIndices;

	UPROPERTY()
	TArray<ACollectableKeyHolder*> KeyHolders;



	// Health
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 Health;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	int32 MaxHealth;




	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, meta = (AllowPrivateAccess = "true"))
	float GameOverDelay;

	FTimerHandle GameOverTimer;


	void FinishDeath();


};