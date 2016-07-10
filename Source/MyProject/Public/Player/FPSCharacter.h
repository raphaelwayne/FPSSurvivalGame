// Copyright 2016, raphaelwayne

#pragma once

#include "FPSProjectile.h"
#include "InventoryComponent.h"
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
	virtual void Tick(float DeltaSeconds) override;

	/** Character owns the inventory component */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Inventory")
	UInventoryComponent* Inventory;

	/* Defines the players restriction to move the camera with his mouse */
	bool bCameraMovementEnabled;

protected:
	/** Called to bind functionality to input */
	virtual void SetupPlayerInputComponent(class UInputComponent* InputComponent) override;

	/** Get actor derived from UsableActor currently looked at by the player */
	class AUsableItem* GetUsableInView();

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

	/*  */
	UFUNCTION()
	void CameraYaw(float fAmount);

	UFUNCTION()
	void CameraPitch(float fAmount);

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
	class AUsableItem* FocusedUsableActor;

	/* Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	USoundBase* FireSound;

	/* AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = Gameplay)
	UAnimMontage* FireAnimation;

	/* Currently gun in hands */
	UPROPERTY(EditDefaultsOnly, Category = "Player Gun")
	class AGun* CurrentGun;

private:
	/* First person camera */
	UPROPERTY(VisibleAnywhere, Category = "Camera")
	UCameraComponent* FirstPersonCameraComponent;

	/* First person view, arms only seen by the player */
	UPROPERTY(VisibleDefaultsOnly, Category = "Mesh")
	USkeletalMeshComponent* FirstPersonMesh;

	/************************************************************************/
	/*								Status                                  */
	/************************************************************************/

	/** Health of the Character, which is determined by the food/thirst level */
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float CharacterHealth;

	/** Stomach space which is filled with food */
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float StomachSpace;

	/** Food which fills up stomach space and hunger bar */
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float CharacterHunger;

	/** Food which fills up stomach space and thirst bar */
	UPROPERTY(VisibleAnywhere, Category = "Status")
	float CharacterThirst;

	/* Multiplier of the mouse movement */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	float fMouseSensivity;

	/* Invertion of the mouse/camera pitch: mouse up = camera down, mouse down = camera up */
	UPROPERTY(EditDefaultsOnly, Category = "Camera")
	int8 n8MouseInvertion;

public:
	/** Public accessor for the character health */
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetCharacterHealth() const { return CharacterHealth; }

	/** Public setter for the character health */
	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetCharacterHealth(float NewCharacterHealth) { CharacterHealth = NewCharacterHealth; }

	/** Public accessor for the  stomach space */
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetStomachSpace() const { return StomachSpace; }

	/** Public setter for the  stomach space*/
	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetStomachSpace(float NewStomachSpace) { StomachSpace = NewStomachSpace; }

	/** Public accessor for the character hunger */
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetCharacterHunger() const { return CharacterHunger; }

	/** Public setter for the character hunger */
	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetCharacterHunger(float NewCharacterHunger) { CharacterHunger = NewCharacterHunger; }

	/** Public accessor for the character thirst */
	UFUNCTION(BlueprintPure, Category = "Status")
	float GetCharacterThirst() const { return CharacterThirst; }

	/** Public setter for the character thirst */
	UFUNCTION(BlueprintCallable, Category = "Status")
	void SetCharacterThirst(float NewCharacterThirst) { CharacterThirst = NewCharacterThirst; }

	/** Use the UsableActor currently in view */
	UFUNCTION(BlueprintCallable, Category = "PlayerAbility")
	void PickupItem();
};
