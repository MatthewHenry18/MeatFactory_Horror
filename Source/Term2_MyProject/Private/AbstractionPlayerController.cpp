// Fill out your copyright notice in the Description page of Project Settings.


#include "AbstractionPlayerController.h"
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
}

void AAbstractionPlayerController::RequestTurn(float AxisValue)
{
	AddYawInput(AxisValue * BaseTurnRate * GetWorld()->GetDeltaSeconds());
}

void AAbstractionPlayerController::RequestJump()
{
	if (GetCharacter())
	{
		GetCharacter()->Jump();
	}
}