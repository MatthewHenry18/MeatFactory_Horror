// Fill out your copyright notice in the Description page of Project Settings.

#include "AbstractionPlayerCharacter.h"
//tantrum
//
#include "GameFramework/CharacterMovementComponent.h"
#include "Kismet/GameplayStatics.h"
#include "DrawDebugHelpers.h"
//
#include "AbstractionPlayerController.h"
#include "WeponProjectile.h"
//
#include "GameFramework/PlayerController.h"
#include "GameFramework/DamageType.h"
#include "HealthComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/InputComponent.h"
#include "DamageHandlerComponent.h"

//debuf trace
static TAutoConsoleVariable<bool> CVarDisplayTrace(
	TEXT("Tantrum.Character.Debug.DisplayTrace"),
	false,
	TEXT("Display Trace"),
	ECVF_Default);

// Sets default values

AAbstractionPlayerCharacter::AAbstractionPlayerCharacter(const FObjectInitializer& ObjectInitializer)
	: Super(ObjectInitializer)
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	DamageHandlerComponent = CreateDefaultSubobject<UDamageHandlerComponent>(TEXT("DamageHandlerComponent"));
	HealthComponent = CreateDefaultSubobject<UHealthComponent>(TEXT("HealthCmponent"));

	ParticleSystemComponent = CreateDefaultSubobject<UParticleSystemComponent>(TEXT("Particle System"));
	ParticleSystemComponent->SetupAttachment(RootComponent);
	//
	//bReplicates = true;
	//SetReplicateMovement(true);
	//
}

// Called when the game starts or when spawned
void AAbstractionPlayerCharacter::BeginPlay()
{
	Super::BeginPlay();	

	//InteractInterface Effects
	//EffectCooldown = DefaultEffectCooldown;

	//use possess/unpossess
	//assigne player controller on begin play
	PC = GetWorld()->GetFirstPlayerController();
}

// Called every frame
void AAbstractionPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	//Interface Effects /////////////
	//if (bIsUnderEffect)
	//{
	//	if (EffectCoolDOwn > 0)
	//	{
	//		EffectCoolDOwn -= DeltaTime;
	//	}
	//	else
	//	{
	//		bIsUnderEffect = false;
	//		EffectCoolDOwn = DefaultEffectCooldown;
	//		EndEffect();
	//	}
	//}
	//////////////////////////////////
}

// Called to bind functionality to input
void AAbstractionPlayerCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	FInputActionBinding* Binding;
	//these functions fire off events 
	//Interactioncomponent listens to them 

	Binding = &PlayerInputComponent->BindAction(FName("InteractionStart"), IE_Pressed, this, &AAbstractionPlayerCharacter::InteractionStartRequested);
	Binding = &PlayerInputComponent->BindAction(FName("InteractionCancel"), IE_Pressed, this, &AAbstractionPlayerCharacter::InteractionCancelRequested);
}

void AAbstractionPlayerCharacter::FellOutOfWorld(const UDamageType& dmgType)
{
	OnDeath(true);
}
////ForAnimation blueprint
const bool AAbstractionPlayerCharacter::IsAlive() const
{
	if (HealthComponent)
	{
		return !HealthComponent->IsDead();
	}
	return false;
}

const float AAbstractionPlayerCharacter::GetCurrentHealth() const
{
	if (HealthComponent)
	{
		return HealthComponent->GetCurrentHealth();
	}
	return 0.0f;
}

/////Dmage
float AAbstractionPlayerCharacter::TakeDamage(float DamageAmount, FDamageEvent const& DamageEvent, AController* EventInstigator, AActor* DamageCauser)
{
	float Damage = Super::TakeDamage(DamageAmount, DamageEvent, EventInstigator, DamageCauser);
	UE_LOG(LogTemp, Warning, TEXT("AAbstractionPlayerCharacter::TakeDamage Damage %.2f"), Damage);
	if (HealthComponent)
	{
		HealthComponent->TakeDamage(Damage);
		if (HealthComponent->IsDead())
		{
			OnDeath(false);
		}
	}
	return Damage;
}

void AAbstractionPlayerCharacter::SetOnFire(float BaseDamage, float DamageTotalTime, float TakeDamageInterval)
{
	if (DamageHandlerComponent)
	{
		DamageHandlerComponent->TakeFireDamage(BaseDamage, DamageTotalTime, TakeDamageInterval);
	}
}

//Death Fubnction
void AAbstractionPlayerCharacter::OnDeath(bool IsFellOut)
{
	APlayerController* PlayerController = GetController<APlayerController>();
	if (PlayerController)
	{
		//play death animation 

		PlayerController->RestartLevel();
	}
}

//Interaction
void AAbstractionPlayerCharacter::InteractionStartRequested()
{
	OnInteractionStartRequested.Broadcast();
}

void AAbstractionPlayerCharacter::InteractionCancelRequested()
{
	OnInteractionCancelRequested.Broadcast();
}

//Has Looked and turned -> BP Implemented

//Throw Request 
/*
void AAbstractionPlayerCharacter::RequestThrowObject()
{
	if (CanThrowObject())
	{

		CharacterThrowState = ECharacterThrowState::Throwing;
		//ignore collisions otherwise throwable hit player capsule
		if (WeponProjectile->GetRootComponent())
		{
			UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(WeponProjectile->GetRootComponent());
			if (RootPrimitiveComponent)
			{
				RootPrimitiveComponent->IgnoreActorWhenMoving(this, true);
			}
		}
		const FVector& Direction = GetMesh()->GetSocketRotation(TEXT("ObjectAttach")).Vector() * ThrowSpeed;
		//const FVector& Direction = GetActorForwardVector() * ThrowSpeed;
		WeponProjectile->Launch(Direction);  // needs an in actor if want to use target
	}
	else
	{
		ResetThrowableObject();
	}
	
}

//Pull Request
void AAbstractionPlayerCharacter::RequestPullObject()
{
	CharacterThrowState = ECharacterThrowState::RequestingPull;
	FVector StartPos = GetActorLocation();
	FVector EndPos = StartPos + (GetActorForwardVector() * 1000.0f);

	EDrawDebugTrace::Type DebugTrace = CVarDisplayTrace->GetBool() ? EDrawDebugTrace::ForOneFrame : EDrawDebugTrace::None;
	FHitResult HitResult;
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), StartPos, EndPos, 70.0f, UEngineTypes::ConvertToTraceType(ECollisionChannel::ECC_Visibility), false, TArray<AActor*>(), DebugTrace, HitResult, true);
	//
	ProcessTraceResult(HitResult);

}

void AAbstractionPlayerCharacter::RequestPullObject(AWeponProjectile* InWeponProjectile)
{	
	//stop pulling  if running
	if (GetVelocity().SizeSquared() < 100.0f)
	{
		if (WeponProjectile && WeponProjectile->Pull(this))
		{
			if (InWeponProjectile && InWeponProjectile->Pull(this))
			{
				CharacterThrowState = ECharacterThrowState::Pulling;
				WeponProjectile = InWeponProjectile;
				//WeponProjectile->ToggleHighlight(false);
			}		
			CharacterThrowState = ECharacterThrowState::Pulling;
			WeponProjectile = nullptr;
		}
	}
}

void AAbstractionPlayerCharacter::ProcessTraceResult(const FHitResult& HitResult)
{
	//check if there was an existing throwable actor
	//remove the hightlight to avoid wrong feedback 
	AWeponProjectile* HitWeponProjectile = HitResult.bBlockingHit ? Cast<AWeponProjectile>(HitResult.GetActor()) : nullptr;
	const bool IsSameActor = (WeponProjectile == HitWeponProjectile);
	const bool IsValidTarget = HitWeponProjectile && HitWeponProjectile->IsIdle();

	//clean up old actor
	if (WeponProjectile && (!IsValidTarget || !IsSameActor))
	{
		WeponProjectile->ToggleHighlight(false);
		WeponProjectile = nullptr;
	}
	//no target, early out
	if (!IsValidTarget)
	{
		return;
	}
	//new target, set the variable and proceed
	if (!IsSameActor)
	{
		WeponProjectile = HitWeponProjectile;
		WeponProjectile->ToggleHighlight(true);
	}
	if (CharacterThrowState == ECharacterThrowState::RequestingPull)
	{
			RequestPullObject(WeponProjectile);
			
			WeponProjectile->ToggleHighlight(false);
			//ThrowableActor = nullptr;
	}
}

//stop pulling request
void AAbstractionPlayerCharacter::RequestStopPullObject()
{
	//if was pulling an object, drop it
	if (CharacterThrowState == ECharacterThrowState::RequestingPull)
	{
		CharacterThrowState = ECharacterThrowState::None;
		//drops the object
		ResetThrowableObject();
	}
}

void AAbstractionPlayerCharacter::ResetThrowableObject()
{
	//drop object
	if (WeponProjectile)
	{
		WeponProjectile->Drop();
	}
	CharacterThrowState = ECharacterThrowState::None;
	WeponProjectile = nullptr;
}

void AAbstractionPlayerCharacter::OnThrowableAttached(AWeponProjectile* InWeponProjectile)
{

	CharacterThrowState = ECharacterThrowState::Attached;
	WeponProjectile = InWeponProjectile;
	MoveIgnoreActorAdd(WeponProjectile);

	//InWeponProjectile->ToggleHighlight(false);
}

void AAbstractionPlayerCharacter::RequestUseObject()
{

	//ApplyEffect_Implementation(WeponProjectile->GetEffectType(), true);
	WeponProjectile->Destroy();
	ResetThrowableObject();
}

//void AAbstractionPlayerCharacter::OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload)
//{
//	//ignore collisions otherwise throwable hit player capsule
//	if (WeponProjectile->GetRootComponent())
///		UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(WeponProjectile->GetRootComponent());
//		if (RootPrimitiveComponent)
//		{
//			RootPrimitiveComponent->IgnoreActorWhenMoving(this, true);
//		}
//	}
	//const FVector& Direction = GetMesh()->GetSocketRotation(TEXT("ObjectAttach")).Vector() * ThrowSpeed;
//	const FVector& Direction = GetActorForwardVector() * ThrowSpeed;
//	WeponProjectile->Launch(Direction);

	//Debug
	//if (CVarDisplayThrowVelocity->GetBool())
	//{
	//	const FVector& Start = GetMesh()->GetSocketLocation(TEXT("ObjectAttach"));
	//	DrawDebugLine(GetWorld(), Start, Start + Direction, FColor::Red, false, 5.0f);
	//}
//}

*/


//------------------------Item pickup-----------------------------//
void AAbstractionPlayerCharacter::HandleItemCollected()
{
	ItemsCollected++;
	// Play Effects here.
	PC->PlayerCameraManager->PlayCameraShake(CamShake, 1.0f);
	PC->PlayDynamicForceFeedback(ForceFeedbackIntensity, ForceFeedbackDuration, true, false, true, false,
		EDynamicForceFeedbackAction::Start);

	ItemCollected();
}

//interact interface Effect 
//void AAbstractionPlayerCharacter::ApplyEffect_Implementation(EEffectType EffectType, bool bIsBuff)
//{

//}

//void AAbstractionPlayerCharacter::EndEffect()
//{
	
//}
