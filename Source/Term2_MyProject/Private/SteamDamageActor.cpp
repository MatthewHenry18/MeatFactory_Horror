// Fill out your copyright notice in the Description page of Project Settings.


#include "SteamDamageActor.h"
#include "DealDamageComponent2.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/CapsuleComponent.h"



// Sets default values
ASteamDamageActor::ASteamDamageActor()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	DealDamageComponent2 = CreateDefaultSubobject<UDealDamageComponent2>(TEXT("DealDamageComponent2"));
	if (DealDamageComponent2-> GetTriggerCapsule())
	{
		RootComponent = DealDamageComponent2->GetTriggerCapsule();
	}
	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("ParticleSystemComponent"));
	ParticleSystemComponent->SetupAttachment(RootComponent);

}

// Called when the game starts or when spawned
void ASteamDamageActor::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASteamDamageActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (DealDamageComponent2)
	{
		CurrentTimer += DeltaTime;
		if (CurrentTimer >= ToggleTime)
		{
			if (ParticleSystemComponent)
			{
				ParticleSystemComponent->ToggleActive();
			}
			DealDamageComponent2->SetActive(!DealDamageComponent2->IsActive());
			CurrentTimer = 0.0f;
		}
	}

}

