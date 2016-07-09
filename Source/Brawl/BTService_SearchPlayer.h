//   <c> Matteo Ludwig 2016

#pragma once

#include "BehaviorTree/BTService.h"
#include "BTService_SearchPlayer.generated.h"


UCLASS()
class BRAWL_API UBTService_SearchPlayer : public UBTService
{
	GENERATED_BODY()
	
public:

	UBTService_SearchPlayer();

	virtual void TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds) override;
	
	
};
