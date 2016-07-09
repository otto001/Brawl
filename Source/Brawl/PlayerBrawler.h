// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Brawler.h"
#include "PlayerBrawler.generated.h"


USTRUCT(BlueprintType)
struct FPlayerForceMovementStruct : public FForceMovementStruct
{
	GENERATED_BODY()


	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bAbortable2;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bInputDependent2;



	void SetForceMovementData(float NewDestination, float NewSpeed, bool NewbWalkBackwards, int32 NewEndDirection = 0, bool NewbAbortable = false, bool NewbInputDependent = false, EBrawlerMovementMode NewNextMovementMode = EBrawlerMovementMode::Walk)
	{
		bInputDependent = NewbInputDependent;
		NextMovementMode = NewNextMovementMode;
	}

	FPlayerForceMovementStruct()
	{
		bAbortable2 = true;
		bInputDependent2 = true;

	}
};





UCLASS()
class BRAWL_API APlayerBrawler : public ABrawler
{
	GENERATED_BODY()
	
public:

	APlayerBrawler();

	virtual void BeginPlay() override;
	virtual void Tick(float DeltaSeconds) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	//Overrides

	void Movement(float Direction);


	//Settings
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float MaxSprintDoubletapDelay = 1.f;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		FPlayerForceMovementStruct ForceMovementData;


private:

	//Sprint
	float SprintDoubletapDelay;
	int8 SprintDoubletapDirection;

	
};
