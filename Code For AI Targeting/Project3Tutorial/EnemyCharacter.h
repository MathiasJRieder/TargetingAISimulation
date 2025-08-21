// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "EnemyCharacter.generated.h"


UCLASS()
class PROJECT3TUTORIAL_API AEnemyCharacter : public ACharacter
{
	GENERATED_BODY()

		UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = LookAt, meta = (AllowPrivateAccess = "true"))
		class USceneComponent* SightSource;

public:
	// Sets default values for this character's properties
	AEnemyCharacter();

	//Change the rotation of the character to face the given actor
	// actor
	bool LookAtActor(AActor* TargetActor);

	// Can we see the given actor?
	bool CanSeeActor(const AActor* TargetActor) const;

	//The class used to spawna  dodgeball object
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, category = Dodgeball)
	TSubclassOf<class ADodgeballProjectile> DodgeballClass;
protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	void ThrowDodgeBall();

	//Whether the enemy can see the player this frame
	bool bCanSeePlayer = false;

	//Whether the enemy could see the player last frame
	bool bPreviousCanSeePlayer = false;

	FTimerHandle ThrowTimerHandle;
	float ThrowingInterval = 2.0F;
	float ThrowingDelay = 0.5F;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Called to bind functionality to input
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

};
