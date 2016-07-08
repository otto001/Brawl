// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Brawler.h"
#include "PlayerBrawler.generated.h"

/**
 * 
 */
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


private:

	//Sprint
	float SprintDoubletapDelay;
	int8 SprintDoubletapDirection;

	
};
