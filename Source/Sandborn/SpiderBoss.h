#pragma once

#include "CoreMinimal.h"
#include "SpiderEnemyPawn.h"
#include "SpiderBoss.generated.h"


UCLASS()
class SANDBORN_API ASpiderBoss : public ASpiderEnemyPawn
{
	GENERATED_BODY()


public:

	ASpiderBoss();

	virtual void HandleStomp() override;


protected:

	virtual void BeginPlay() override;


private:

	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Boss|Health", meta = (AllowPrivateAccess = "true", ClampMin = "1"))
	int32 MaxHealth;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Boss|Health", meta = (AllowPrivateAccess = "true"))
	int32 Health;
};