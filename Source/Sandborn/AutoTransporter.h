#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "AutoTransporter.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class SANDBORN_API UAutoTransporter : public UActorComponent
{
	GENERATED_BODY()

public:	
	
	UAutoTransporter();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector StartPoint;

	UPROPERTY(BlueprintReadWrite, VisibleAnywhere)
	FVector EndPoint;

	bool ArePointsSet;

	bool MovingToEnd;

	UPROPERTY(EditAnywhere)
	float MoveTime;

	UFUNCTION(BlueprintCallable)
	void SetPoints(FVector Point1, FVector Point2);

		
};
