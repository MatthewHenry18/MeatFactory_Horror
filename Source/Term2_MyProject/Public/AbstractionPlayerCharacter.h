// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
//#include "IDetailTreeNode.h"
#include "GameFramework/Character.h"
#include "AbstractionPlayerCharacter.generated.h"



class UDamageHandlerComponent;
class UHealthComponent;
class UParticleSystemComponent;

//these are input bindings
DECLARE_MULTICAST_DELEGATE(FInteractionStartRequest);
DECLARE_MULTICAST_DELEGATE(FInteractionCancelRequest);

UCLASS()
class TERM2_MYPROJECT_API AAbstractionPlayerCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	//AAbstractionPlayerCharacter();

	/*Default UObject Constructor*/
	AAbstractionPlayerCharacter(const FObjectInitializer& ObjectInitializer = FObjectInitializer::Get());
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*Called When actor fall out of world safely (below kill z)*/
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;   //overide
	
	UFUNCTION(BlueprintCallable, Category = "Abstraction")
		void SetOnFire(float BaseDamage, float DamageTotalTime, float TakeDamageInterval);
	
	//Item Pickup 
	UFUNCTION(BlueprintCallable)
		void HandleItemCollected();

	UFUNCTION(BlueprintImplementableEvent)
		void ItemCollected();

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
		int ItemsCollected = 0;

	//bindings, a hack atm as the interactble components in the game world get the player and sign themselves up to these events
	//to know when the player has pressed the input binding for interacting
	FInteractionStartRequest OnInteractionStartRequested;
	FInteractionCancelRequest OnInteractionCancelRequested;

	UFUNCTION(BlueprintImplementableEvent)
		void DoorOpenInteractionStarted(AActor* InteractableActor);

	//can be array or moved later as needed
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

	/*//////////////////////////////////   */

	UPROPERTY(EditAnywhere, Category = "Effects")
		TSubclassOf<UMatineeCameraShake> CamShake;

	/*////////////////////////////////////////*/

	// Force Feedback values.
	UPROPERTY(EditAnywhere, Category = "Force Feedback")
		float ForceFeedbackIntensity = 1.0f;
	UPROPERTY(EditAnywhere, Category = "Force Feedback")
		float ForceFeedbackDuration = 1.0f;




};
