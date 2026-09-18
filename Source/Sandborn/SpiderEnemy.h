#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "SpiderEnemy.generated.h"

UCLASS()
class SANDBORN_API ASpiderEnemy : public ACharacter
{
	GENERATED_BODY()

public:
	
	ASpiderEnemy();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
