// Fill out your copyright notice in the Description page of Project Settings.


#include "WeponProjectile.h"
#include "Components/StaticMeshComponent.h"
#include "GameFramework/Character.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "AbstractionPlayerCharacter.h"
#include "GameFramework/ProjectileMovementComponent.h"


AWeponProjectile::AWeponProjectile()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
	bReplicates = true;
	SetReplicateMovement(true);

	//creating the components we are going to be using (mesh and projectile) -> h file 
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>("StaticMeshComponent");
	ProjectileMovementComponent = CreateDefaultSubobject<UProjectileMovementComponent>("ProjectileMovementComponent");
	RootComponent = StaticMeshComponent;
}

void AWeponProjectile::BeginPlay()
{
	Super::BeginPlay();
	if (HasAuthority())
	{
		ProjectileMovementComponent->OnProjectileStop.AddDynamic(this, &AWeponProjectile::ProjectileStop);
	}
}

void AWeponProjectile::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	if (HasAuthority())
	{
		ProjectileMovementComponent->OnProjectileStop.RemoveDynamic(this, &AWeponProjectile::ProjectileStop);
	}
	Super::EndPlay(EndPlayReason);
}

//driving the attach in blueprintv  - > Virtual Actor Function we override
void AWeponProjectile::NotifyHit(UPrimitiveComponent* MyComp, AActor* Other, UPrimitiveComponent* OtherComp, bool bSelfMoved, FVector HitLocation, FVector HitNormal, FVector NormalImpulse, const FHitResult& Hit)
{
	Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit);   // call super to get actor benefits

	//potentially early out if not authoritive

	if (State == EState::Idle || State == EState::Attached || State == EState::Dropped)
	{
		return;
	}
	// EFFECTS
	/*
	if (State == EState::Launch)
	{
		IInteractInterface* I = Cast<IInteractInterface>(Other);
		if (I)
		{
			I->Execute_ApplyEffect(Other, EffectType, false);
		}
	}
	*/

	if (PullActor && State == EState::Pull)
	{
		//
		//Super::NotifyHit(MyComp, Other, OtherComp, bSelfMoved, HitLocation, HitNormal, NormalImpulse, Hit)
			//
		if (AAbstractionPlayerCharacter* AbstractionPlayerCharacter = Cast<AAbstractionPlayerCharacter>(PullActor))
		{

			if (Other == PullActor)
			{
				//simulating attach state
				AttachToComponent(AbstractionPlayerCharacter->GetMesh(), FAttachmentTransformRules::SnapToTargetNotIncludingScale, TEXT("ObjectAttach"));
				SetOwner(AbstractionPlayerCharacter);
				ProjectileMovementComponent->Deactivate();
				State = EState::Attached;
				//set character state to attached
				AbstractionPlayerCharacter->OnThrowableAttached(this);  // tell character item attache dtoit
			}
			else
			{
				AbstractionPlayerCharacter->ResetThrowableObject();
				State = EState::Dropped;
			}
		}
	}
	

	ProjectileMovementComponent->HomingTargetComponent = nullptr;
	PullActor = nullptr;
}

//only called on authority
void AWeponProjectile::ProjectileStop(const FHitResult& ImpactResult)
{
	if (State == EState::Launch || State == EState::Dropped)
	{
		State = EState::Idle;
	}
}

bool AWeponProjectile::Pull(AActor* InActor)
{
	if (State != EState::Idle)
	{
		return false;
	}
	//set homing target call activate 
	if (SetHomingTarget(InActor))
	{
		ToggleHighlight(false);
		State = EState::Pull;
		PullActor = InActor;
		return true;
	}

	return false;
}
//call this functiion on actor 
void AWeponProjectile::Launch(const FVector& InitialVelocity, AActor* Target )// can pass in a targrt
{
	if (State == EState::Pull || State == EState::Attached)
	{
		//detach from actor 
		DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		//clear out homing target
		ProjectileMovementComponent->Activate(true);
		ProjectileMovementComponent->HomingTargetComponent = nullptr;

		State = EState::Launch;

		if (Target)
		{
			if (USceneComponent* SceneComponent = Cast<USceneComponent>(Target->GetComponentByClass(USceneComponent::StaticClass())))
			{
				ProjectileMovementComponent->HomingTargetComponent = TWeakObjectPtr<USceneComponent>(SceneComponent);
				return;
			}
		}

		ProjectileMovementComponent->Velocity = InitialVelocity;
	}
}

void AWeponProjectile::Drop()
{
	if (State == EState::Pull || State == EState::Attached)
	{
		if (State == EState::Attached)
		{
			DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		}

		ProjectileMovementComponent->Activate(true);
		ProjectileMovementComponent->HomingTargetComponent = nullptr;
		State = EState::Dropped;
	}
}

void AWeponProjectile::ToggleHighlight(bool bIsOn)
{
	StaticMeshComponent->SetRenderCustomDepth(bIsOn);
}
//EFFECTS

//EEffectType AThrowableActor::GetEffectType()
//{
//	return EffectType;
//}



bool AWeponProjectile::SetHomingTarget(AActor* Target)
{
	if (Target)
	{
		if (USceneComponent* SceneComponent = Cast<USceneComponent>(Target->GetComponentByClass(USceneComponent::StaticClass())))
		{
			if (USceneComponent* ThrowableSceneComponent = Cast<USceneComponent>(GetComponentByClass(USceneComponent::StaticClass())))
			{//calls activate, set the scene component, set homing component and gives a velocity 
				ProjectileMovementComponent->SetUpdatedComponent(ThrowableSceneComponent);
				ProjectileMovementComponent->Activate(true);
				ProjectileMovementComponent->HomingTargetComponent = TWeakObjectPtr<USceneComponent>(SceneComponent);
				//should be exposed for easy adjustment
				ProjectileMovementComponent->Velocity = FVector(0.0f, 0.0f, 1000.0f);
				return true;
			}
		}
	}

	return false;
}

