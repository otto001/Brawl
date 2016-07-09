// <c> Matteo Ludwig 2016

#include "Brawl.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "AiBrawlerController.h"
#include "AiBrawler.h"

#include "PlayerBrawler.h"

#include "BTTask_BrawlerMoveTo.h"



EBTNodeResult::Type UBTTask_BrawlerMoveTo::ExecuteTask(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory)
{
	AAiBrawlerController* BrawlerController = Cast<AAiBrawlerController>(OwnerComp.GetAIOwner());
	AAiBrawler* Brawler = Cast<AAiBrawler>(BrawlerController->GetPawn());

	//APlayerBrawler* Player = Cast<APlayerBrawler>(OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Object>(BrawlerController->KeyId_Player));

	FVector Destination = OwnerComp.GetBlackboardComponent()->GetValue<UBlackboardKeyType_Vector>(BrawlerController->KeyId_Destination);

	if ((Destination - Brawler->GetActorLocation()).Size() > BrawlerController->AcceptableDistance)
	{










		return EBTNodeResult::Succeeded;
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 5.f, FColor::Red, "There");
		return EBTNodeResult::Failed;
	}
	return EBTNodeResult::Failed;
}
