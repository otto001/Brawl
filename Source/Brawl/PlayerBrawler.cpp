// Fill out your copyright notice in the Description page of Project Settings.

#include "Brawl.h"
#include "PlayerBrawler.h"



APlayerBrawler::APlayerBrawler()
{
}

void APlayerBrawler::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	InputComponent->BindAxis("Movement", this, &APlayerBrawler::Movement);

	InputComponent->BindAction("Sprint", IE_Pressed, this, &ABrawler::StartSprint);
	InputComponent->BindAction("Sprint", IE_Released, this, &ABrawler::EndSprint);

	InputComponent->BindAction("Jump", IE_Pressed, this, &ABrawler::StartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &ABrawler::EndJump);


}

void APlayerBrawler::BeginPlay()
{
	Super::BeginPlay();

}

void APlayerBrawler::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void APlayerBrawler::Movement(float Direction)
{
	float DeltaTime = GetWorld()->GetDeltaSeconds();


	

	if (!bSprint)
	{
		//SprintDoubletapTimer
		if (SprintDoubletapDelay > MaxSprintDoubletapDelay || SprintDoubletapDirection == -Direction)
		{
			SprintDoubletapDelay = SprintDoubletapDirection = 0;
		}
		else if (SprintDoubletapDelay > 0)
		{
			SprintDoubletapDelay += DeltaTime;
		}
		
		UE_LOG(LogTemp, Warning, TEXT("%d, %f, %f"), SprintDoubletapDirection, Direction, SprintDoubletapDelay);

		//Doubletap Sprint init
		if (SprintDoubletapDirection != 0 && Direction == 0 && SprintDoubletapDelay == 0)
		{
			SprintDoubletapDelay = DeltaTime;
		}
		else if (SprintDoubletapDelay > 0 && SprintDoubletapDirection == Direction)
		{

			ChangeMovementMode(EBrawlerMovementMode::Sprint, EChangeModeSetting::Add);
			
			SprintDoubletapDelay = 0;

		}

		if (SprintDoubletapDelay == 0)
		{
			if (Direction > 0)
			{
				SprintDoubletapDirection = 1;
			}
			else if (Direction < 0)
			{
				SprintDoubletapDirection = -1;
			}
			else
			{
				SprintDoubletapDirection = 0;
			}
		}
		

	}
	else if (SprintDoubletapDelay != 0)
	{
		SprintDoubletapDelay = 0;
	}
	Super::Movement(Direction);
}