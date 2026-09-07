#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "AutoTransporter.h"
#include "Components/StaticMeshComponent.h"
#include "Components/ArrowComponent.h"
#include "AutoMovingPlatform.generated.h"

UCLASS()
class SANDBORN_API AAutoMovingPlatform : public AActor
{
	GENERATED_BODY()
	
public:	
	
	AAutoMovingPlatform();

protected:
	
	virtual void BeginPlay() override;

public:	
	
	virtual void Tick(float DeltaTime) override;

};
