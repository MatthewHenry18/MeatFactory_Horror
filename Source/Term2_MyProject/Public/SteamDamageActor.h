// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "SteamDamageActor.generated.h"

class UDealDamageComponent2;
class UParticleSystemComponent;

UCLASS()
class TERM2_MYPROJECT_API ASteamDamageActor : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	ASteamDamageActor();
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UPROPERTY(EditAnywhere)
	float ToggleTime = 5.0f;

	float CurrentTimer = 0.0f;

	UPROPERTY(EditAnywhere)
	UDealDamageComponent2* DealDamageComponent2;
	UPROPERTY(EditAnywhere)
	UParticleSystemComponent* ParticleSystemComponent;

};
