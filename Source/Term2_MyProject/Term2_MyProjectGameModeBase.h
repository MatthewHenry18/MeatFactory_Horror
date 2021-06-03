// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "Blueprint/UserWidget.h"
#include "Term2_MyProjectGameModeBase.generated.h"

//to retrieve this object
// AGameModeBase* GameMode = Cast<Term2_MyProjectGameModeBase>(World->GetAuthGameMode());

UCLASS()
class TERM2_MYPROJECT_API ATerm2_MyProjectGameModeBase : public AGameModeBase
{
	GENERATED_BODY()

public:
	ATerm2_MyProjectGameModeBase() {}

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> ObjectiveWidgetClass;

	UPROPERTY(EditDefaultsOnly)
		TSubclassOf<UUserWidget> ObjectivesCompleteWidgetClass;

		
};
