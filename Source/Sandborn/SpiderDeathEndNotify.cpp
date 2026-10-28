#include "SpiderDeathEndNotify.h"
#include "SpiderEnemyPawn.h"
#include "Components/SkeletalMeshComponent.h"


void USpiderDeathEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	ASpiderEnemyPawn* Spider =	Cast<ASpiderEnemyPawn>(MeshComp->GetOwner());
	if (!Spider) return;

	Spider->CompleteDeath();
}