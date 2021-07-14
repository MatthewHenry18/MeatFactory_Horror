// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractionPlayerController.h"
#include "AbstractionPlayerCharacter.h"
#include "GameFramework/Character.h"

//can use controller to tell character

void AAbstractionPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();
	if (InputComponent)
	{
		//tellling the controller there is inpout
		InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Pressed, this, &AAbstractionPlayerController::RequestJump);
		//InputComponent->BindAction(TEXT("Jump"), EInputEvent::IE_Released, this, &AAbstractionPlayerController::RequestStopJump);

		InputComponent->BindAxis(TEXT("MoveForward"), this, &AAbstractionPlayerController::RequestMoveForward);
		InputComponent->BindAxis(TEXT("MoveRight"), this, &AAbstractionPlayerController::RequestMoveRight);
		InputComponent->BindAxis(TEXT("LookUp"), this, &AAbstractionPlayerController::RequestLookUp);
		InputComponent->BindAxis(TEXT("Turn"), this, &AAbstractionPlayerController::RequestTurn);

		//pull + THrow
		//InputComponent->BindAction(TEXT("PullObject"), EInputEvent::IE_Pressed, this, &AAbstractionPlayerController::RequestPullObject);
		//InputComponent->BindAction(TEXT("PullObject"), EInputEvent::IE_Released, this, &AAbstractionPlayerController::RequestStopPullObject);

		//InputComponent->BindAxis(TEXT("ThrowObject"), this, &AAbstractionPlayerController::RequestThrowObject);
	}
}

void AAbstractionPlayerController::RequestMoveForward(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::X), AxisValue);
	}
}

void AAbstractionPlayerController::RequestMoveRight(float AxisValue)
{
	if (AxisValue != 0.f)
	{
		FRotator const ControlSpaceRot = GetControlRotation();
		// transform to world space and add it
		GetPawn()->AddMovementInput(FRotationMatrix(ControlSpaceRot).GetScaledAxis(EAxis::Y), AxisValue);
	}
}

void AAbstractionPlayerController::RequestLookUp(float AxisValue)
{
	AddPitchInput(AxisValue * BaseLookUpRate * GetWorld()->GetDeltaSeconds());
	
	
	if (AAbstractionPlayerCharacter* AbstractionPlayerCharacter = Cast<AAbstractionPlayerCharacter>(GetCharacter()))
	{

		AbstractionPlayerCharacter->HasLookedUp();
	}
}

void AAbstractionPlayerController::RequestTurn(float AxisValue)
{
	AddYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());

	if (AAbstractionPlayerCharacter* AbstractionPlayerCharacter = Cast<AAbstractionPlayerCharacter>(GetCharacter()))
	{

		AbstractionPlayerCharacter->HasTurned();
	}
}

void AAbstractionPlayerController::RequestJump()
{

	if (GetCharacter())
	{
		GetCharacter()->Jump();
	}
}
/*
void AAbstractionPlayerController::RequestPullObject()
{

	if (AAbstractionPlayerCharacter* AbstractionPlayerCharacter = Cast<AAbstractionPlayerCharacter>(GetCharacter()))
	{

		AbstractionPlayerCharacter->RequestPullObject();
	}
}

void AAbstractionPlayerController::RequestStopPullObject()
{
	if (AAbstractionPlayerCharacter* AbstractionPlayerCharacter = Cast<AAbstractionPlayerCharacter>(GetCharacter()))
	{
		AbstractionPlayerCharacter->RequestStopPullObject();
	}
}

void AAbstractionPlayerController::RequestThrowObject(float AxisValue)
{
	if (AAbstractionPlayerCharacter* AbstractionPlayerCharacter = Cast<AAbstractionPlayerCharacter>(GetCharacter()))
	{
		if (AbstractionPlayerCharacter->CanThrowObject()) 
		{
			float currentDelta = AxisValue - LastAxis;

			LastAxis = AxisValue;
				const bool IsFLick = fabs(currentDelta) > FlickThreshold;
				if (IsFLick)
				{
					AbstractionPlayerCharacter->RequestThrowObject();
				}
		}
		else
		{
			LastAxis = 0.0f;
		}
	}
	//on notify begin recieved 
}
*/