// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"

class UDamageHandlerComponent;
class UHealthComponent;
class UParticleSystemComponent;


UCLASS()
class TERM2_MYPROJECT_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	/*Called When actor fall out of world safely (below kill z)*/
	virtual void FellOutOfWorld(const class UDamageType& dmgType) override;

	virtual float TakeDamage(float DamageAmount, struct FDamageEvent const& DamageEvent, class AController* EventInstigator, AActor* DamageCauser) override;   //overide

	UFUNCTION(BlueprintCallable, Category = "Abstraction")
		void SetOnFire(float BaseDamage, float DamageTotalTime, float TakeDamageInterval);

	//For animation
	UFUNCTION(BlueprintCallable)
		const bool IsAlive() const;

	//Player on fire particles - can be array or moved later as needed
	UPROPERTY(EditAnywhere)
		UParticleSystemComponent* ParticleSystemComponent;

	UFUNCTION(BlueprintCallable)
		const float GetCurrentHealth() const;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void OnDeath(bool IsFellOut);

	UPROPERTY(EditAnywhere)
		UHealthComponent* HealthComponent;

	UPROPERTY(EditAnywhere)
		UDamageHandlerComponent* DamageHandlerComponent;

};
