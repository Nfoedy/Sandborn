#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SpiderAttackHitNotify.generated.h"


UCLASS()
class SANDBORN_API USpiderAttackHitNotify : public UAnimNotify
{
	GENERATED_BODY()

public:
	 
	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};