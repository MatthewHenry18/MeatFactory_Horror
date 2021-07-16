// Fill out your copyright notice in the Description page of Project Settings.


#include "DoorInteraction.h"
#include "GameFrameWork/Actor.h" 
#include "GameFramework/PlayerController.h"
#include "Engine/World.h"
#include "Engine/Engine.h"
#include "DrawDebugHelpers.h"
#include "ObjectiveComponent.h"
#include "InteractionComponent.h"
#include "Components/AudioComponent.h"
#include "Components/TextRenderComponent.h"
#include "AbstractionPlayerCharacter.h"

constexpr float FLT_METERS(float meters) { return meters * 100.f; }

static TAutoConsoleVariable<bool> CVarToggleDebugDoor(
	TEXT("Abstraction.DoorInteraction.Debug"),
	false,
	TEXT("Toggle DoorInteraction debug display."),
	ECVF_Default);

// Sets default values for this component's properties
UDoorInteraction::UDoorInteraction()
{
	PrimaryComponentTick.bCanEverTick = true;
	DoorState = EDoorState::DS_Closed;

	CVarToggleDebugDoor.AsVariable()->SetOnChangedCallback(FConsoleVariableDelegate::CreateStatic(&UDoorInteraction::OnDebugToggled));
}

void UDoorInteraction::TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);

	if (DoorState == EDoorState::DS_Opening)
	{
		CurrentRotationTime += DeltaTime;
		const float TimeRatio = FMath::Clamp(CurrentRotationTime / TimeToRotate, 0.0f, 1.0f);
		const float RotationAlpha = OpenCurve.GetRichCurveConst()->Eval(TimeRatio);
		const FRotator CurrentRotation = FMath::Lerp(StartRotation, FinalRotation, RotationAlpha);
		GetOwner()->SetActorRotation(CurrentRotation);
		if (TimeRatio >= 1.0f)
		{
			//called internally when door is finished opening
			OnDoorOpen();
		}
	}

	DebugDraw();
}

void UDoorInteraction::OpenDoor()
{
	if (IsOpen() || DoorState == EDoorState::DS_Opening)
	{
		return;
	}
	//
	//if (AudioComponent)
	//{
	//	AudioComponent->Play();
	//}

	DoorState = EDoorState::DS_Opening;
	CurrentRotationTime = 0.0f;
}

void UDoorInteraction::BeginPlay()
{
	Super::BeginPlay();
	StartRotation = GetOwner()->GetActorRotation();
	FinalRotation = GetOwner()->GetActorRotation() + DesiredRotation;
	//ensure TimeToRotate is greater than EPSILON
	CurrentRotationTime = 0.0f;

	AudioComponent = GetOwner()->FindComponentByClass<UAudioComponent>();
	//check(AudioComponent);																??
	if (!AudioComponent)
	{
		UE_LOG(LogTemp, Warning, TEXT("UDoorInteractionComponent::BeginPlay() Missing Audio Component"));
	}

	TextRenderComponent = GetOwner()->FindComponentByClass<UTextRenderComponent>();
}

void UDoorInteraction::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("UDoorInteractionComponent::OnOverlapBegin"));
	//we already have somebody interacting, currently we don't support multiple interactions
	if (InteractingActor || !bActive)
	{
		return;
	}

	//for now we will get that component and set visible
	if (OtherActor->ActorHasTag("Player"))
	{
		InteractingActor = OtherActor;
		if (TextRenderComponent)
		{
			TextRenderComponent->SetText(InteractionPrompt);
			TextRenderComponent->SetVisibility(true);
		}
	}
}

void UDoorInteraction::OnOverlapEnd(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	UE_LOG(LogTemp, Warning, TEXT("UDoorInteractionComponent::OnOverlapEnd"));
	if (OtherActor == InteractingActor)
	{
		InteractingActor = nullptr;
		if (TextRenderComponent)
		{
			TextRenderComponent->SetVisibility(false);
		}
	}
}

void UDoorInteraction::InteractionRequested()
{
	//ideally we would make sure this is allowed
	if (InteractingActor)
	{
		bActive = false;
		if (TextRenderComponent)
		{
			TextRenderComponent->SetText(InteractionPrompt);
			TextRenderComponent->SetVisibility(false);
		}

		AAbstractionPlayerCharacter* APC = Cast<AAbstractionPlayerCharacter>(InteractingActor);
		if (APC)
		{
			APC->DoorOpenInteractionStarted(GetOwner());
		}

		//this will be called from the owner to be in sync with animation
		//OpenDoor();
	}
}



void UDoorInteraction::OnDoorOpen()
{
	DoorState = EDoorState::DS_Open;
	UObjectiveComponent* ObjectiveComponent = GetOwner()->FindComponentByClass<UObjectiveComponent>();
	if (ObjectiveComponent)
	{
		ObjectiveComponent->SetState(EObjectiveState::OS_Completed);
	}

	//hide prompt
	//disable interaction

	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("DoorOpened"));
	//tell any listeners that the interaction is successful
	InteractionSuccess.Broadcast();
}

void UDoorInteraction::OnDebugToggled(IConsoleVariable* Var)
{
	UE_LOG(LogTemp, Warning, TEXT("OnDebugToggled"));
}

void UDoorInteraction::DebugDraw()
{
	if (CVarToggleDebugDoor->GetBool())
	{
		FVector Offset(FLT_METERS(-0.75f), 0.0f, FLT_METERS(2.5f));
		FVector StartLocation = GetOwner()->GetActorLocation() + Offset;
		FString EnumAsString = TEXT("Door State: ") + UEnum::GetDisplayValueAsText(DoorState).ToString();
		DrawDebugString(GetWorld(), Offset, EnumAsString, GetOwner(), FColor::Blue, 0.0f);
	}
}