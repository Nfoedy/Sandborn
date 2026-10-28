#include "SpiderEnemy.h"


ASpiderEnemy::ASpiderEnemy()
{
 	
	PrimaryActorTick.bCanEverTick = true;

}


void ASpiderEnemy::BeginPlay()
{
	Super::BeginPlay();
	
}


void ASpiderEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


void ASpiderEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

