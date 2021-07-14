// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "InteractInterface.h"
//#include "IDetailTreeNode.h"
#include "GameFramework/Character.h"
#include "AbstractionPlayerCharacter.generated.h"



class UDamageHandlerComponent;
class UHealthComponent;
class UParticleSystemComponent;


//these are input bindings
DECLARE_MULTICAST_DELEGATE(FInteractionStartRequest);
DECLARE_MULTICAST_DELEGATE(FInteractionCancelRequest);

UCLASS()                                                                  //to include interface to class
class TERM2_MYPROJECT_API AAbstractionPlayerCharacter : public ACharacter//, public IInteractInterface
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	/*Default UObject Constructor*/
	AAbstractionPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	//AAbstractionPlayerCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*Called When actor fall out of world safely (below kill z)*/
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;   //overide
	
	//UFUNCTION(BlueprintCallable, Category = "Abstraction")
	//	void SetOnFire(float BaseDamage, float DamageTotalTime, float TakeDamageInterval);
	
	//Item Pickup //////////////////////////////////
	UFUNCTION(BlueprintCallable)
		void HandleItemCollected();

	UFUNCTION(BlueprintImplementableEvent)
		void ItemCollected();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int ItemsCollected = 0;
	//-////////////////////////////////////////////////

	//bindings, a hack atm as the interactble components in the game world get the player and sign themselves up to these events
	//to know when the player has pressed the input binding for interacting
	FInteractionStartRequest OnInteractionStartRequested;
	FInteractionCancelRequest OnInteractionCancelRequested;



	//Controller Has Look/turned 
	UFUNCTION(BlueprintImplementableEvent)
		void HasLookedUp();
	UFUNCTION(BlueprintImplementableEvent)
		void HasTurned();

	//////------Door Interaction-------/////
	UFUNCTION(BlueprintImplementableEvent)
		void DoorOpenInteractionStarted(AActor* InteractableActor);

	//Player on fire particles - can be array or moved later as needed
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* ParticleSystemComponent;

	//For animation
	UFUNCTION(BlueprintCallable)
		const bool IsAlive() const;

	UPROPERTY(EditAnywhere)
		float SprintSpeed = 3056.0f;

	UPROPERTY(EditAnywhere)
		float WalkSpeed = 2045.0f;

	UFUNCTION(BlueprintCallable)
		const float GetCurrentHealth() const;
	//
	//UPROPERTY(VisibleAnywhere, ReplicatedUsing = OnRep_CharacterThrowState, Category = "Throw")
	//	UPROPERTY(VisibleAnywhere, /*replicated, */ Category = "Throw")
	//	ECharacterThrowState CharacterThrowState = ECharacterThrowState::None;
	///
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDeath(bool IsFellOut);

	//Input Bindings
	void InteractionStartRequested();
	void InteractionCancelRequested();

	UPROPERTY(EditAnywhere)
		UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere)
		UDamageHandlerComponent* DamageHandlerComponent;

	APlayerController* PC;

	UPROPERTY(EditAnywhere, Category = "Effects")
		TSubclassOf<UMatineeCameraShake> CamShake;

	// Force Feedback values.
	UPROPERTY(EditAnywhere, Category = "Force Feedback")
		float ForceFeedbackIntensity = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Force Feedback")
		float ForceFeedbackDuration = 1.0f;

	//bool bIsStunned = false;
	//bool bIsSprinting = false;

	float MaxWalkSpeed = 0.0f;

	//
	UPROPERTY(EditAnywhere, Category = "Throw", meta = (ClampMin = "0.0", Unit = "ms"))
		float ThrowSpeed = 2000.0f;


	private:

};

//class AWeponProjectile;

//Throwing enum G
/*
UENUM(BlueprintType)
enum class ECharacterThrowState : uint8
{
	None			UMETA(DisplayName = "None"),
	RequestingPull	UMETA(DisplayName = "RequestingPull"),
	Pulling			UMETA(DisplayName = "Pulling"),
	Attached		UMETA(DisplayName = "Attached"),
	Throwing		UMETA(DisplayName = "Throwing"),
};
*/

//UPROPERTY()
//	AWeponProjectile* WeponProjectile;


//Expective Implementation bc BLueprint Native
//void ApplyEffect_Implementation(EEffectType EffectType, bool bIsBuff) override;

//void EndEffect();

//bool bIsUnderEffect = false;
//bool bIsEffectBuff = false;

//float DefaultEffectCooldown = 5.0f;
//float EffectCoolDOwn = 0.0f;

//THROW BINDINGS 
/*
	//setting the triggers to bindings
	void RequestThrowObject();
	void RequestPullObject(AWeponProjectile* InWeponProjectile);
	void RequestPullObject();

	void ProcessTraceResult(const FHitResult& HitResult);


	void RequestStopPullObject();
	void ResetThrowableObject();

	void RequestUseObject();

	void OnThrowableAttached(AWeponProjectile* InWeponProjectile);
//
	bool CanThrowObject() const { return CharacterThrowState == ECharacterThrowState::Attached; }

	UFUNCTION(BlueprintPure)
		bool IsPullingObject() const { return CharacterThrowState == ECharacterThrowState::RequestingPull || CharacterThrowState == ECharacterThrowState::Pulling; }

	UFUNCTION(BlueprintPure)
		bool IsThrowing() const { return CharacterThrowState == ECharacterThrowState::Throwing; }

	UFUNCTION(BlueprintPure)
		ECharacterThrowState GetCharacterThrowState() const { return CharacterThrowState; }

	//where throw happens
	//UFUNCTION()
	//	void OnNotifyBeginReceived(FName NotifyName, const FBranchingPointNotifyPayload& BranchingPointNotifyPayload);
	*/
	//	