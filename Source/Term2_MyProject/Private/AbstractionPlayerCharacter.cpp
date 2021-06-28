// Fill out your copyright notice in the Description page of Project Settings.

#include "AbstractionPlayerCharacter.h"
//tantrum
//#include "GameFramework/CharacterMovementComponent.h"
//#include "Kismet/GameplayStatics.h"
//#include "AbstractionPlayerController.h"
#include "WeponProjectile.h"
//
#include "GameFramework/PlayerController.h"
#include "GameFramework/DamageType.h"
#include "HealthComponent.h"
#include "Particles/ParticleSystemComponent.h"
#include "Components/InputComponent.h"
#include "DamageHandlerComponent.h"

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
	//use possess/unpossess
	//assigne player controller on begin play
	PC = GetWorld()->GetFirstPlayerController();
}

// Called every frame
void AAbstractionPlayerCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
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

//Throw Request 

void AAbstractionPlayerCharacter::RequestThrowObject()
{
	if (CanThrowObject()) //()
	{
		CharacterThrowState = ECharacterThrowState::Throwing;

		//ignore collisions otherwise the throwable object hits the player capsule and doesn't travel in the desired direction
		if (WeponProjectile->GetRootComponent())
		{
			UPrimitiveComponent* RootPrimitiveComponent = Cast<UPrimitiveComponent>(WeponProjectile->GetRootComponent());
			if (RootPrimitiveComponent)
			{
				RootPrimitiveComponent->IgnoreActorWhenMoving(this, true);
			}
		}
		//const FVector& Direction = GetMesh()->GetSocketRotation(TEXT("ObjectAttach")).Vector() * -ThrowSpeed;
		const FVector& Direction = GetActorForwardVector() * ThrowSpeed;
		WeponProjectile->Launch(Direction);

		//Debug Draw
		//if (CVarDisplayThrowVelocity->GetBool())
		//{
		//	const FVector& Start = GetMesh()->GetSocketLocation(TEXT("ObjectAttach"));
			//DrawDebugLine(GetWorld(), Start, Start + Direction, FColor::Red, false, 5.0f);
		//}
	}
}

//Pull Request
void AAbstractionPlayerCharacter::RequestPullObject(AWeponProjectile* InWeponProjectile)
{
	//make sure we are in idle
	if (!bIsStunned && CharacterThrowState == ECharacterThrowState::None)
	{
		CharacterThrowState = ECharacterThrowState::RequestingPull;
		if (InWeponProjectile && InWeponProjectile->Pull(this))
		{
			CharacterThrowState = ECharacterThrowState::Pulling;
			InWeponProjectile = InWeponProjectile;
			WeponProjectile->ToggleHighlight(false);
		}
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
	//InThrowableActor->ToggleHighlight(false);
}

void AAbstractionPlayerCharacter::RequestUseObject()
{
	//ApplyEffect_Implementation(WeponProjectile->GetEffectType(), true);
	WeponProjectile->Destroy();
	ResetThrowableObject();
}


//throwable outline
void AAbstractionPlayerCharacter::ProcessTraceResult(const FHitResult& HitResult)
{
	//called at specific moment in anim montage 
//character hand a animation motage slot
}


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
