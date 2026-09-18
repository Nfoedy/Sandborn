#include "SpiderAttackEndNotify.h"
#include "SpiderEnemyPawn.h"
#include "Components/SkeletalMeshComponent.h"


void USpiderAttackEndNotify::Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference)
{
	Super::Notify(MeshComp, Animation, EventReference);

	if (!MeshComp) return;

	ASpiderEnemyPawn* Spider = Cast<ASpiderEnemyPawn>(MeshComp->GetOwner());
	if (!Spider) return;

	Spider->EndAttack();
}