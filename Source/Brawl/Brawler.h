//  <c> Matteo Ludwig 2016

#pragma once

#include "GameFramework/Character.h"
#include "Brawler.generated.h"

UENUM(BlueprintType)
enum class EBrawlerMovementMode : uint8
{
	Walk 	UMETA(DisplayName = "Walk"),
	Crouch 	UMETA(DisplayName = "Crouch"),
	Sprint 	UMETA(DisplayName = "Sprint"),
	Falling UMETA(DisplayName = "Falling"),
	Land UMETA(DisplayName = "Land"),
	WallHold 	UMETA(DisplayName = "WallHold"),
	WallSlide 	UMETA(DisplayName = "WallSlide"),
	EdgeHold 	UMETA(DisplayName = "EdgeHold"),
	EdgeClimb	UMETA(DisplayName = "EdgeClimb"),
	Vault	UMETA(DisplayName = "Vault"),
	ForceMovement UMETA(DisplayName = "ForceMovement"),
};

UENUM(BlueprintType)
enum class EChangeModeSetting : uint8
{
	Add			UMETA(DisplayName = "Add"),
	RemoveOnly 	UMETA(DisplayName = "RemoveOnly"),
	Override 	UMETA(DisplayName = "Override"),
	SetOnly 	UMETA(DisplayName = "SetOnly"),
};

UENUM(BlueprintType)
enum class EBrawlerAction : uint8
{
	Jump 	UMETA(DisplayName = "Jump"),
};

USTRUCT(BlueprintType)
struct FForceMovementStruct
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Destination;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float Speed;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bWalkBackwards;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		int32 EndDirection;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bAbortable;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bInputDependent;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		EBrawlerMovementMode NextMovementMode;

	float GetRemainingDistance(AActor* ReferenceActor)
	{
		return FMath::Abs(Destination - ReferenceActor->GetActorLocation().Y);
	}

	void SetForceMovementData(float NewDestination, float NewSpeed, bool NewbWalkBackwards, int32 NewEndDirection = 0, bool NewbAbortable = false, bool NewbInputDependent = false, EBrawlerMovementMode NewNextMovementMode = EBrawlerMovementMode::Walk)
	{
		Destination = NewDestination;
		Speed = NewSpeed;
		bWalkBackwards = NewbWalkBackwards;
		EndDirection = NewEndDirection;
		bAbortable = NewbAbortable;
		bInputDependent = NewbInputDependent;
		NextMovementMode = NewNextMovementMode;
	}

	FForceMovementStruct()
	{
		Destination = 0;
		Speed = 0;
		EndDirection = 0;
		bAbortable = true;
		bInputDependent = true;
		NextMovementMode = EBrawlerMovementMode::Walk;
	}
};

UCLASS()
class BRAWL_API ABrawler : public ACharacter
{
	GENERATED_BODY()

public:

	ABrawler();

	virtual void BeginPlay() override;
	virtual void Tick( float DeltaSeconds ) override;


	//Movement

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		EBrawlerMovementMode MovementMode;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		bool bSprint;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		bool bWalkBackwards;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		FForceMovementStruct ForceMovementData;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float SprintSpeed = 500;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float WalkSpeed = 250;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float WalkBackwardsSpeed = 250;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float WallHoldTime = 0.33f;;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float WallSlideSpeed = 0.8f;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		bool bOnGround;
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Movement)
		bool bWalljumped;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float ArmLength = 16.f;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Parkour)
		float ScanningTolerance = 2.f;
	


	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = EdgeClimb)
		UCurveFloat* EdgeClimbMontageCurve;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = EdgeClimb)
		float EdgeClimbCurrentHeight;

	/*UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vault)
		float VaultMontageCurrentOffset;*/
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vault)
		float VaultMontageCurrentThickness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vault)
		float VaultMontageCurrentHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vault)
		float VaultThickness;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vault)
		float VaultHeight;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vault)
		float VaultScanDistance;
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Vault)
		float VaultDistance;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Movement)
		float ImmobileTimer;

	UFUNCTION(BlueprintCallable, Category = Movement)
	void ChangeMovementMode(EBrawlerMovementMode NewMode, EChangeModeSetting Setting);

	UFUNCTION(BlueprintCallable, Category = Movement)
	FVector GetDirection();
	FVector CurrentDirection;


	void BrawlerJump();
	void BrawlerAction(EBrawlerAction NewAction);

	//UFUNCTION(BlueprintImplementableEvent, Category = Movement)
	//	void EdgeClimb();
	


	void Movement(float Direction);
	void StartSprint();
	void EndSprint();
	void StartJump();
	void EndJump();

	

private:
	

	void ObstacleScan();
	bool GetOnGround();
	/*void PressCrouch();
	void ReleaseCrouch();*/

	//State bools
	bool bJump;
	

	//Walljump
	bool bOnWall;

	//WallHold
	float WallHoldTimer;


	bool VaultScan();

	
};
