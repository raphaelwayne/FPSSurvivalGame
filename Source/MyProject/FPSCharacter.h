// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "FPSProjectile.h"
#include "UsableItem.h"
#include "InventoryItem.h"
#include "GameFramework/Character.h"
#include "FPSCharacter.generated.h"

#define print(DebugString) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::Red, DebugString)

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

	UPROPERTY(VisibleAnywhere)
	TArray<AInventoryItem*> CharacterInventory;

	const int32 MaxInventorySlots = 4;

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
	AUsableItem* FocusedUsableActor;

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
	virtual void PickupItem();
};
