//   <c> Matteo Ludwig 2016

#include "Brawl.h"

#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"

#include "AiBrawlerController.h"
#include "AiBrawler.h"

#include "PlayerBrawler.h"

#include "BTService_SearchPlayer.h"


UBTService_SearchPlayer::UBTService_SearchPlayer()
{
	bCreateNodeInstance = true;
}

void UBTService_SearchPlayer::TickNode(UBehaviorTreeComponent& OwnerComp, uint8* NodeMemory, float DeltaSeconds)
{
	AAiBrawlerController* BrawlerController = Cast<AAiBrawlerController>(OwnerComp.GetAIOwner());

	if (BrawlerController)
	{
		APlayerBrawler* Player = Cast<APlayerBrawler>(GetWorld()->GetFirstPlayerController()->GetPawn());

		if (Player)
		{
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Object>(BrawlerController->KeyId_Player, Player);
			OwnerComp.GetBlackboardComponent()->SetValue<UBlackboardKeyType_Vector>(BrawlerController->KeyId_Destination, Player->GetActorLocation());
		}

	}

}