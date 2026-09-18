#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SpiderAttackEndNotify.generated.h"


UCLASS()
class SANDBORN_API USpiderAttackEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};
