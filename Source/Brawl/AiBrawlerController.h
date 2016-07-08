//  <c> Matteo Ludwig 2016


#pragma once

#include "AIController.h"
#include "AiBrawlerController.generated.h"


UCLASS()
class BRAWL_API AAiBrawlerController : public AAIController
{
	GENERATED_BODY()
	
	UPROPERTY(transient)
	class UBlackboardComponent* BlackboardComponent;
	
	UPROPERTY(transient)
	class UBehaviorTreeComponent* BehaviorTreeComponent;

public:

	AAiBrawlerController();

	virtual void Possess(APawn* InPawn) override;
	
	uint8 AiKeyId;
};
