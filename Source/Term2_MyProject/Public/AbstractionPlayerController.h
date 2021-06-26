// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbstractionPlayerController.generated.h"


UCLASS()
class TERM2_MYPROJECT_API AAbstractionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	void SetupInputComponent() override;
	
	void RequestMoveForward(float AxisValue);
	void RequestMoveRight(float AxisValue);
	void RequestLookUp(float AxisValue);
	void RequestTurn(float AxisValue);
	void RequestJump();

	UPROPERTY(EditAnywhere, Category = "Look")
		float BaseLookUpRate = 90.0f;

	UPROPERTY(EditAnywhere, Category = "Look")
		float BaseTurnRate = 90.0f;



	//AAbstractionPlayerController() {}

	APlayerCameraManager* PlayerCameraManager;
	
};
