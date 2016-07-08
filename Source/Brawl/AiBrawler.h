//  <c> Matteo Ludwig 2016


#pragma once

#include "Brawler.h"
#include "AiBrawler.generated.h"


UCLASS()
class BRAWL_API AAiBrawler : public ABrawler
{
	GENERATED_BODY()
	
public:

	AAiBrawler();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Behavior)
	class UBehaviorTree* BehaviorTree;
	
};
