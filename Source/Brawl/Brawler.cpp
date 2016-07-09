//  <c> Matteo Ludwig 2016

//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Red, " ");


#include "Brawl.h"
#include "Brawler.h"
#include "DrawDebugHelpers.h"

ABrawler::ABrawler()
{
	PrimaryActorTick.bCanEverTick = true;
}


void ABrawler::BeginPlay()
{
	Super::BeginPlay();
	
}

void ABrawler::Tick(float DeltaTime)
{
	Super::Tick( DeltaTime );

	CurrentDirection = GetDirection();
	bOnGround = GetOnGround();


	if (GetVelocity().Y != 0 && (int(MovementMode) < 4 || MovementMode == EBrawlerMovementMode::ForceMovement))
	{
		float Direction = 1;
		if (GetVelocity().Y < 0)
		{
			Direction = -1;
		}
		if (bWalkBackwards)
		{
			Direction *= -1;
		}
		SetActorRotation(FRotator(0, Direction * FMath::Clamp(GetMovementComponent()->GetMaxSpeed(), -1.f, 1.f) * 90, 0));
	}

	ObstacleScan();

	if (bOnGround)
	{
		bWalljumped = false;
		bOnWall = false;

		if (MovementMode == EBrawlerMovementMode::Falling || MovementMode == EBrawlerMovementMode::WallSlide)
		{	
			ChangeMovementMode(EBrawlerMovementMode::Land, EChangeModeSetting::Override);
		}
	}
	else
	{
		if (MovementMode != EBrawlerMovementMode::Falling)
		{
			ChangeMovementMode(EBrawlerMovementMode::Falling, EChangeModeSetting::Add);
		}
	}


	if (MovementMode == EBrawlerMovementMode::WallHold)
	{
		if (WallHoldTimer > 0)
		{
			WallHoldTimer -= DeltaTime;
		}
		else
		{
			ChangeMovementMode(EBrawlerMovementMode::WallSlide, EChangeModeSetting::Override);
		}
	}

	if (ImmobileTimer > 0)
	{
		ImmobileTimer -= DeltaTime;
		if (ImmobileTimer <= 0)
		{
			ImmobileTimer = 0;
		}
	}

	if (MovementMode == EBrawlerMovementMode::ForceMovement)
	{
		FVector CurrentLocation = GetActorLocation();
		if (ForceMovementData.GetRemainingDistance(this) > ForceMovementData.Speed * DeltaTime)
		{
			AddMovementInput(FVector(0, -1, 0), FMath::Clamp(CurrentLocation.Y - ForceMovementData.Destination, -1.f, 1.f));
		}
		else
		{
			SetActorLocation(FVector(CurrentLocation.X, ForceMovementData.Destination, CurrentLocation.Z));
			if (ForceMovementData.EndDirection != 0 && CurrentDirection.Y != ForceMovementData.EndDirection)
			{
				AddActorLocalRotation(FRotator(0, 180, 0));
			}
			ChangeMovementMode(ForceMovementData.NextMovementMode, EChangeModeSetting::Override);
		}
	}

}


void ABrawler::StartSprint()
{
	bSprint = true;
	ChangeMovementMode(EBrawlerMovementMode::Sprint, EChangeModeSetting::Add);
}

void ABrawler::EndSprint()
{
	bSprint = false;
	ChangeMovementMode(EBrawlerMovementMode::Sprint, EChangeModeSetting::RemoveOnly);
}

void ABrawler::StartJump()
{
	bJump = true;
}

void ABrawler::EndJump()
{
	bJump = false;
}


FVector ABrawler::GetDirection()
{
	FVector Direction = FVector::ZeroVector;
	Direction.Y = 1;
	if (GetActorForwardVector().Y < 0)
	{
		Direction.Y = -1;
	}
	return Direction;
}

bool ABrawler::GetOnGround()
{
	float CapsuleHalfHeight, CapsuleRadius;
	GetCapsuleComponent()->GetScaledCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	FHitResult BoxTrace;

	FVector Start = GetActorLocation() + FVector(0, 0, -CapsuleHalfHeight);
	FVector End = Start + FVector(0, 0, -5);

	GetWorld()->SweepSingleByObjectType(BoxTrace, Start, End, FQuat(0, 0, 0, 0), ObjectParams, FCollisionShape::MakeBox(FVector(1, CapsuleRadius - 2, 1)), CollisionParams);

	return BoxTrace.bBlockingHit;
}

void ABrawler::Movement(float Direction)
{

	if (Direction == 0 && bSprint)
	{
		ChangeMovementMode(EBrawlerMovementMode::Sprint, EChangeModeSetting::RemoveOnly);
	}
	if (ImmobileTimer <= 0)
	{

		if (MovementMode == EBrawlerMovementMode::EdgeHold && Direction != 0 && Direction != CurrentDirection.Y)
		{
			ChangeMovementMode(EBrawlerMovementMode::Falling, EChangeModeSetting::Override);
		}

		if (MovementMode == EBrawlerMovementMode::ForceMovement)
		{

			if (ForceMovementData.bAbortable && ((Direction != CurrentDirection.Y && Direction != 0) || (ForceMovementData.bInputDependent && Direction == 0)))
			{
				ChangeMovementMode(EBrawlerMovementMode::Walk, EChangeModeSetting::Override);
			}
		}
		else if (Direction != 0)
		{
			if (bWalkBackwards)
			{
				Direction *= -1;
			}
			AddMovementInput(FVector(0, 1, 0), Direction);
		}

	}
}

void ABrawler::ChangeMovementMode(EBrawlerMovementMode NewMode, EChangeModeSetting Setting)
{

	EBrawlerMovementMode RemoveMode = MovementMode;
	if (Setting == EChangeModeSetting::RemoveOnly)
	{
		RemoveMode = NewMode;
		if (int(NewMode) > int(MovementMode))
		{
			return;
		}
	}

	if (Setting == EChangeModeSetting::Override && NewMode == EBrawlerMovementMode::Walk)
	{

		if (int(MovementMode) <= int(EBrawlerMovementMode::Sprint))
		{
			return;
		}
		if (bSprint)
		{
			NewMode = EBrawlerMovementMode::Sprint;
		}

	}

	if (Setting == EChangeModeSetting::RemoveOnly && NewMode == EBrawlerMovementMode::Sprint && MovementMode == EBrawlerMovementMode::Sprint)
	{
		NewMode = EBrawlerMovementMode::Walk;
	}

	if (Setting != EChangeModeSetting::SetOnly && Setting != EChangeModeSetting::Add)
	{
		switch (RemoveMode)
		{
		case EBrawlerMovementMode::Walk:
			break;

		case EBrawlerMovementMode::Crouch:
			break;

		case EBrawlerMovementMode::Sprint:
			bSprint = false;
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;

			break;

		case EBrawlerMovementMode::ForceMovement:
			bWalkBackwards = false;
			break;

		case EBrawlerMovementMode::Falling:
			break;

		case EBrawlerMovementMode::Land:
			break;

		case EBrawlerMovementMode::WallHold:
			GetCharacterMovement()->GravityScale = 1;
			WallHoldTimer = 1;
			break;

		case EBrawlerMovementMode::WallSlide:
			GetCharacterMovement()->GravityScale = 1;
			break;

		
		case EBrawlerMovementMode::EdgeHold:
			GetCharacterMovement()->GravityScale = 1;
			break;

		case EBrawlerMovementMode::EdgeClimb:
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			break;

		case EBrawlerMovementMode::Vault:
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Walking);
			break;
		}
	}

	if (NewMode != MovementMode && (Setting != EChangeModeSetting::RemoveOnly || NewMode == EBrawlerMovementMode::Walk))
	{
		switch (NewMode)
		{
		case EBrawlerMovementMode::Walk:
			GetCharacterMovement()->MaxWalkSpeed = WalkSpeed;
			bSprint = false;
			break;

		case EBrawlerMovementMode::Crouch:
			break;

		case EBrawlerMovementMode::Sprint:
			GetCharacterMovement()->MaxWalkSpeed = SprintSpeed;
			bSprint = true;
			break;

		case EBrawlerMovementMode::ForceMovement:
			GetCharacterMovement()->MaxWalkSpeed = ForceMovementData.Speed;
			bWalkBackwards = ForceMovementData.bWalkBackwards;
			break;

		case EBrawlerMovementMode::Falling:
			break;

		case EBrawlerMovementMode::Land:
			break;

		case EBrawlerMovementMode::WallHold:
			if (MovementMode != EBrawlerMovementMode::WallSlide)
			{
				GetCharacterMovement()->GravityScale = 0;
				GetCharacterMovement()->Velocity = FVector::ZeroVector;
				WallHoldTimer = WallHoldTime;


			}
			break;

		case EBrawlerMovementMode::WallSlide:
			GetCharacterMovement()->GravityScale = WallSlideSpeed;
			break;

		case EBrawlerMovementMode::EdgeHold:
			//<<<<<<<<<<<<<<<<<<<< Potential interferance with obstacle scan tolerance system >>>>>>>>>>>>>>>>>>>
			GetCharacterMovement()->GravityScale = 0;
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			break;

		case EBrawlerMovementMode::EdgeClimb:
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			GetCharacterMovement()->Velocity = FVector::ZeroVector;

			/*if (!GetMesh()->GetAnimInstance()->Montage_IsActive(EdgeClimbHighMontage) && !GetMesh()->GetAnimInstance()->Montage_IsActive(EdgeClimbLowMontage))
			{
				GetMesh()->GetAnimInstance()->Montage_Play(EdgeClimbHighMontage, 1);
			}*/
			break;

		case EBrawlerMovementMode::Vault:
			GetCharacterMovement()->SetMovementMode(EMovementMode::MOVE_Flying);
			GetCharacterMovement()->Velocity = FVector::ZeroVector;
			break;
		}
	}

	if ((Setting != EChangeModeSetting::Add && Setting != EChangeModeSetting::RemoveOnly) ||
		(Setting == EChangeModeSetting::Add && int(NewMode) > int(MovementMode)) ||
		(Setting == EChangeModeSetting::RemoveOnly && NewMode == EBrawlerMovementMode::Walk) /// <- Why?-----------------------------------------------------
		)
	{
		MovementMode = NewMode;
	}

}


void ABrawler::ObstacleScan()
{
	//<<<<<<<<<<<<<<<<<<<< Tolerance system: beware of multiple edge detection -> changemode >>>>>>>>>>>>>>>>>>>
	FVector Direction = CurrentDirection;

	float CapsuleHalfHeight, CapsuleRadius;
	GetCapsuleComponent()->GetScaledCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	if (bSprint || bJump && GetVelocity().Y != 0)
	{
		VaultScan();
	}

	

	// Edge & Wallslide
	if (MovementMode != EBrawlerMovementMode::Vault)
	{
		if (bJump)
		{
			BrawlerAction(EBrawlerAction::Jump);

		}

		FHitResult BoxTrace;

		FVector BoxStart = GetActorLocation() + Direction * (CapsuleRadius + ScanningTolerance/2);
		FVector BoxEnd = BoxStart + FVector(0, 0, 10 - CapsuleHalfHeight);
		BoxStart += FVector(0, 0, CapsuleHalfHeight + ArmLength);

		GetWorld()->SweepSingleByObjectType(BoxTrace, BoxStart, BoxEnd, FQuat(0,0,0,0), ObjectParams, FCollisionShape::MakeBox(FVector(1, ScanningTolerance, 1)), CollisionParams);
		//DrawDebugBox(GetWorld(), FMath::Lerp(BoxStart, BoxEnd ,0.5f), FVector(1, ScanningTolerance, (BoxStart.Z - BoxEnd.Z)/2), FColor::Red);

		if (BoxTrace.bBlockingHit)
		{
			//DrawDebugSphere(GetWorld(), BoxTrace.Location, 10, 10, FColor::Green);

			if (BoxTrace.Location.Z > GetActorLocation().Z + CapsuleHalfHeight)
			{
				if (!bOnGround)
				{
					FHitResult OverheadHitResult;

					FVector OverheadStart = GetActorLocation() + Direction*CapsuleRadius + FVector(0, 0, CapsuleHalfHeight + ArmLength + ScanningTolerance);
					FVector OverheadEnd = OverheadStart + Direction * (ScanningTolerance*2);

					GetWorld()->LineTraceSingleByObjectType(OverheadHitResult, OverheadStart, OverheadEnd, ObjectParams, CollisionParams);
					if (OverheadHitResult.bBlockingHit)
					{
						bOnWall = true;
						if (GetVelocity().Z <= 0)
						{
							FVector Start = GetActorLocation() + Direction * CapsuleRadius;
							FVector End = Start + Direction * ScanningTolerance;

							FHitResult TopHitResult;
							FHitResult BottomHitResult;


							GetWorld()->LineTraceSingleByObjectType(TopHitResult, Start + FVector(0, 0, CapsuleHalfHeight), End + FVector(0, 0, CapsuleHalfHeight) , ObjectParams, CollisionParams);
							GetWorld()->LineTraceSingleByObjectType(BottomHitResult, Start + FVector(0, 0, -CapsuleHalfHeight), End + FVector(0, 0, -CapsuleHalfHeight), ObjectParams, CollisionParams);

							if (TopHitResult.bBlockingHit && BottomHitResult.bBlockingHit)
							{
								ChangeMovementMode(EBrawlerMovementMode::WallHold, EChangeModeSetting::Add);
							}
							else
							{
								bOnWall = false;
								ChangeMovementMode(EBrawlerMovementMode::WallSlide, EChangeModeSetting::RemoveOnly);
							}

						}
					}

					else if (MovementMode != EBrawlerMovementMode::EdgeClimb)
					{
						FHitResult TopHitResult;

						FVector TopStart = GetActorLocation() + FVector(0, 0, CapsuleHalfHeight);
						FVector TopEnd = TopStart + FVector(0, 0, ScanningTolerance + ArmLength);

						GetWorld()->SweepSingleByObjectType(TopHitResult, TopStart, TopEnd, FQuat(0, 0, 0, 0), ObjectParams, FCollisionShape::MakeBox(FVector(1, CapsuleRadius -  2, 1)), CollisionParams);
						DrawDebugLine(GetWorld(), TopStart, TopEnd, FColor::Emerald, true, 10);

						if (!TopHitResult.bBlockingHit)
						{
							ChangeMovementMode(EBrawlerMovementMode::EdgeHold, EChangeModeSetting::Add);


							AddActorWorldOffset(FVector(0, 0, BoxTrace.Location.Z - (GetActorLocation().Z + CapsuleHalfHeight + ArmLength)));

							EdgeClimbCurrentHeight = (BoxTrace.Location.Z - GetActorLocation().Z + CapsuleHalfHeight) / (CapsuleHalfHeight * 2 + ArmLength);
						}
						bOnWall = false;
					}
				}

			}
			else if ((bJump && int(MovementMode) <= 3) || MovementMode == EBrawlerMovementMode::Falling)
			{
				float Height = BoxTrace.Location.Z - GetActorLocation().Z + CapsuleHalfHeight;
				ChangeMovementMode(EBrawlerMovementMode::EdgeClimb, EChangeModeSetting::Add);
				EdgeClimbCurrentHeight = Height / (CapsuleHalfHeight * 2 + ArmLength);
			}
		}
		else
		{
			bOnWall = false;
			ChangeMovementMode(EBrawlerMovementMode::WallSlide, EChangeModeSetting::RemoveOnly);
		}
	}
	
}

bool ABrawler::VaultScan()
{
	FVector Direction = CurrentDirection;

	float CapsuleHalfHeight, CapsuleRadius;
	GetCapsuleComponent()->GetScaledCapsuleSize(CapsuleRadius, CapsuleHalfHeight);

	FCollisionObjectQueryParams ObjectParams;
	ObjectParams.AddObjectTypesToQuery(ECC_WorldStatic);

	FCollisionQueryParams CollisionParams;
	CollisionParams.AddIgnoredActor(this);

	bool IsVault = false;

	//Vault
	if (int(MovementMode) < 3)
	{
		FVector CurrentLocation = GetActorLocation();

		FHitResult VaultTrace;

		//Trace 1
		FVector Start = CurrentLocation + Direction * CapsuleRadius + FVector(0, 0, 10 - CapsuleHalfHeight);
		FVector End = Start + Direction * (VaultScanDistance + ScanningTolerance);


		GetWorld()->LineTraceSingleByObjectType(VaultTrace, Start, End, ObjectParams, CollisionParams);

		if (!VaultTrace.bBlockingHit)
		{
			return false;
		}
		FVector FirstHit = VaultTrace.Location;

		

		//Trace 2
		Start = End = VaultTrace.Location + Direction * ScanningTolerance + FVector(0, 0, 10);
		Start.Z += VaultHeight + ScanningTolerance;
		
		GetWorld()->LineTraceSingleByObjectType(VaultTrace, Start, End, ObjectParams, CollisionParams);

		float Height = VaultTrace.Location.Z - CurrentLocation.Z + CapsuleHalfHeight;

		if (!VaultTrace.bBlockingHit || !(Height < VaultHeight))
		{
			return false;
		}

		//Trace 3
		Start = FirstHit + Direction * (CapsuleRadius + VaultThickness + ScanningTolerance);
		End = FirstHit;
		
		GetWorld()->LineTraceSingleByObjectType(VaultTrace, Start, End, ObjectParams, CollisionParams);

		float Thickness = FMath::Abs(FirstHit.Y - VaultTrace.Location.Y);
		
		if (Thickness <= VaultThickness && VaultTrace.Location.Y != Start.Y)
		{
			float Destination = FirstHit.Y - (CapsuleRadius + VaultDistance)*Direction.Y;
			float Distance = -(CurrentLocation.Y - Destination) * Direction.Y;

			//DrawDebugCapsule(GetWorld(), CurrentLocation, 88, 25, FQuat(0, 0, 0, 0), FColor::Red, true, 10);

			DrawDebugPoint(GetWorld(), FVector(CurrentLocation.X, Destination, CurrentLocation.Z), 10, FColor::Emerald, true, 10);

			//GEngine->AddOnScreenDebugMessage(-1, 1.f, FColor::Emerald, FString::SanitizeFloat(Distance));

			
			VaultMontageCurrentThickness = Thickness;
			VaultMontageCurrentHeight = Height;

			if (Distance > 0)
			{
				ForceMovementData.SetForceMovementData(Destination, GetMovementComponent()->GetMaxSpeed(), false, Direction.Y, true, true, EBrawlerMovementMode::Vault);
			}
			else
			{

				ForceMovementData.SetForceMovementData(Destination, WalkSpeed, true, Direction.Y, false, false, EBrawlerMovementMode::Vault);
			}
			
			ChangeMovementMode(EBrawlerMovementMode::ForceMovement, EChangeModeSetting::Add);
			

			return true;
		}
	}
	return false;
}


void ABrawler::BrawlerAction(EBrawlerAction NewAction)
{
	switch (NewAction)
	{
	case EBrawlerAction::Jump:
		BrawlerJump();
		break;
	}
}


void ABrawler::BrawlerJump()
{
	if(ImmobileTimer <= 0)
	{
		if (bOnWall)
		{
			if (!bWalljumped)
			{
				LaunchCharacter((FVector(0, 0, 1) - 1.f*CurrentDirection) * GetCharacterMovement()->JumpZVelocity, true, true);
				bWalljumped = true;
				//AddControllerPitchInput(180);
				//AddActorLocalRotation(FRotator(0, 180, 0));
				ChangeMovementMode(EBrawlerMovementMode::Falling, EChangeModeSetting::Override);
			}
		}
		else if (MovementMode == EBrawlerMovementMode::EdgeHold)
		{
			EdgeClimbCurrentHeight = 1;
			ChangeMovementMode(EBrawlerMovementMode::EdgeClimb, EChangeModeSetting::Add);
		}
		else
		{
			if (MovementMode != EBrawlerMovementMode::Land && MovementMode != EBrawlerMovementMode::ForceMovement)
			{
				Jump();
			}
		}
	}
}