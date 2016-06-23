// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSProjectile.h"
#include "UsableActor.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

UCLASS()
class MYPROJECT_API AFPSCharacter : public ACharacter
{
	GENERATED_BODY()

public:

	/** Sets default values for this character's properties 
	 *  The overloaded constructor here ensures, that each instance of the class gets constructed with initialized properties.
	 */
	AFPSCharacter();

	/** Called when the game starts or when spawned */
	virtual void BeginPlay() override;
	
	/** Called every frame */
	virtual void Tick( float DeltaSeconds ) override;

protected:

	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Get actor derived from UsableActor currently looked at by the player */
	class AUsableActor* GetUsableInView();

	/************************************************************************/
	/*								Movement                                */
	/************************************************************************/
	
	/** Called to move forward/backward */
	void MoveForward(float Val);

	/** Called to move right/left */
	void MoveRight(float Val);

	/** Set jumpflag to active */
	void OnStartJump();

	/** Clear jumpflag */
	void OnStopJump();

	/************************************************************************/
	/*								Gameplay                                */
	/************************************************************************/

	/** Handles firing */
	UFUNCTION()
	void OnFire();

	/* Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AFPSProjectile> ProjectileClass;

	/* Gun muzzle's offset from the FPSCamera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleOffset;

	/* Max distance to use the UsableActor */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	float MaxTraceDistance;	

	/* Only true in first frame when focused on new UsableActor*/
	bool bHasNewFocus;

	/* Actor currently in center view */
	AUsableActor* FocusedUsableActor;

private:
	/* First person camera */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FirstPersonCameraComponent;

	/* First person view, arms only seen by the player */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FirstPersonMesh;

public:
	/** Use the UsableActor currently in view */
	UFUNCTION(BlueprintCallable, Category = "PlayerAbility")
	virtual void Use();
};
