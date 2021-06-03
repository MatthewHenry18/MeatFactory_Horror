// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "AbstractionPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class TERM2_MYPROJECT_API AAbstractionPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	AAbstractionPlayerController() {}

	APlayerCameraManager* PlayerCameraManager;
	
};
