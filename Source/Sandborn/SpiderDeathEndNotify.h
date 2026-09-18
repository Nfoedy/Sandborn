#pragma once

#include "CoreMinimal.h"
#include "Animation/AnimNotifies/AnimNotify.h"
#include "SpiderDeathEndNotify.generated.h"


UCLASS()
class SANDBORN_API USpiderDeathEndNotify : public UAnimNotify
{
	GENERATED_BODY()

public:

	virtual void Notify(USkeletalMeshComponent* MeshComp, UAnimSequenceBase* Animation, const FAnimNotifyEventReference& EventReference) override;
};