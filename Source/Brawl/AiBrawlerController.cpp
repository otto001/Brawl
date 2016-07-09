//  <c> Matteo Ludwig 2016


#include "Brawl.h"
#include "BehaviorTree/BehaviorTree.h"
#include "BehaviorTree/BehaviorTreeComponent.h"
#include "BehaviorTree/BlackboardComponent.h"
#include "BehaviorTree/Blackboard/BlackboardKeyAllTypes.h"
#include "AiBrawlerController.h"
#include "AiBrawler.h"



AAiBrawlerController::AAiBrawlerController()
{
	BlackboardComponent = CreateDefaultSubobject<UBlackboardComponent>(TEXT("BlackboardComponent"));
	BehaviorTreeComponent = CreateDefaultSubobject<UBehaviorTreeComponent>(TEXT("BehaviorTreeComponent"));
}

void AAiBrawlerController::Possess(APawn * InPawn)
{
	Super::Possess(InPawn);

	AAiBrawler* Brawler = Cast<AAiBrawler>(InPawn);

	if (Brawler && Brawler->BehaviorTree)
	{
		BlackboardComponent->InitializeBlackboard(*Brawler->BehaviorTree->BlackboardAsset);

		KeyId_Player = BlackboardComponent->GetKeyID("Player");

		BehaviorTreeComponent->StartTree(*Brawler->BehaviorTree);
	}

}