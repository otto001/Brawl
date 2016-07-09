// <c> Matteo Ludwig 2016

#pragma once

#include "BehaviorTree/Tasks/BTTask_BlackboardBase.h"
#include "BTTask_BrawlerMoveTo.generated.h"

/**
 * 
 */
UCLASS()
class BRAWL_API UBTTask_BrawlerMoveTo : public UBTTask_BlackboardBase
{
	GENERATED_BODY()
	
public:

	virtual EBTNodeResult::Type ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory) override;
	
	
};
