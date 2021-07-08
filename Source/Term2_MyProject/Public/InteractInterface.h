// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "InteractInterface.generated.h"

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UInteractInterface : public UInterface
{
	GENERATED_BODY()
};

UENUM(BlueprintType)
enum class EEffectType : uint8
{
	None		 UMETA(DisplayName = "None"),
	Speed		 UMETA(DisplayName = "SpeedBuff"),
	Jump		 UMETA(DisplayName = "JumpBuff"),
	Power		 UMETA(DisplayName = "PowerBuff"),
};

class TERM2_MYPROJECT_API IInteractInterface
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void ApplyEffect(EEffectType EffectType, bool bIsBuff);



	///EXAMPLES///
	//BlueprintCallable so that we can implement it via BP and BlueprintNativeEvent so we can also implement base functionality
	//int a C++ class

	//Typed function so we can implement it into a function in the BP Class rather than an Event in EventGraph
	/*
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
	const bool TypedInteract();  // const allows us to not use return
	//Example without type
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void NonTypedInteract();
	//Example to show how to use signature
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent, Category = "Interact")
		void SignatureInteract(bool& Return);
	*/
};
