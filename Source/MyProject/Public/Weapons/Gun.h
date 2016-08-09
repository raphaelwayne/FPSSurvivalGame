// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "UsableItem.h"
#include "Gun.generated.h"

USTRUCT()
struct FWeaponData
{
	GENERATED_USTRUCT_BODY()

	/* Max ammo for player */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Ammo")
	int32 MaxAmmo;

	/* Ammo in clip */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Ammo")
	int32 ClipAmmo;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Ammo")
	int32 InitialClips;

	/* Shots per second */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Ammo")
	float FireRate;

	FWeaponData()
	{
		MaxAmmo = 100;
		ClipAmmo = 20;
		InitialClips = 4;
		FireRate = 0.2f;
	}
};

USTRUCT()
struct FWeaponAnimation
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Animation")
	UAnimMontage* Animation1P;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Animation")
	UAnimMontage* Animation3P;
};

USTRUCT()
struct FWeaponSound
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sounds")
	USoundBase* FireLoopSound1P;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sounds")
	USoundBase* FireLoopSound3P;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sounds")
	USoundBase* FireBurstSound1P;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sounds")
	USoundBase* FireBurstSound3P;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sounds")
	USoundBase* FireSingleSound1P;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sounds")
	USoundBase* FireSingleSound3P;
};

UENUM()
enum class EWeaponState
{
	Idle,
	Firing,
	Reloading,
	Equipping,
};

UENUM()
enum class EFireMode
{
	Automatic,
	Burst,
	Single
};

/* Player has primary and/or secondary gun */
UENUM()
enum class EGunType
{
	Primary,
	Secondary
};

UCLASS(Abstract, Blueprintable)
class MYPROJECT_API AGun : public AUsableItem
{
	GENERATED_BODY()
	
	/* Member-Variables */
protected:
	/* Weapon properties */
	UPROPERTY(EditDefaultsOnly, Category = "Weapon Properties")
	FWeaponData WeaponProperties;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Animations")
	FWeaponAnimation WeaponAnimations;

	UPROPERTY(EditDefaultsOnly, Category = "Weapon Sounds")
	FWeaponSound WeaponSounds;

	/* Player currently owning this weapon */
	ACharacter* OwningPawn;

	/* If i would make this boolean, i would assign 1 byte to each boolean. Since this class will have a lot of this, i can save some space by creating bit fields of "uint32 booleans" */
	uint32 bIsFiring : 1;
	
	/* Is weapon reloading? */
	uint32 bIsReloading : 1;

	/* Is weapon doing nothing? */
	uint32 bIsIdling : 1;

	/* Is weapon equipped? */
	uint32 bIsEquipped : 1;

	/* Is weapon firing? */
	uint32 bWantsToFire : 1;

	EWeaponState CurrentState;

	EFireMode CurrentMode;

	/* Projectile class to spawn */
	UPROPERTY(EditDefaultsOnly, Category = "Projectile")
	TSubclassOf<class AFPSProjectile> Projectile;

	/* Gun muzzle's offset from the FPSCamera */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Gameplay")
	FVector MuzzleLocation;

	/* TimerHandle for firing the gun */
	FTimerHandle TimerHandle_HandleWeaponFire;

private:
	/* Mesh third person */
	UPROPERTY(VisibleDefaultsOnly, Category = "Weapon Mesh")
	USkeletalMeshComponent* Mesh3P;

	/** Member-Functions */
public:
	AGun();

	/* Return current firemode of weapon */
	EFireMode GetCurrentMode() const;

	/* Return current state of weapon */
	EWeaponState GetCurrentState() const;

	/* Set the owning player */
	void SetOwningPawn(ACharacter* Character);

	/* Return owner of the weapon */
	ACharacter* GetOwningPawn() const;

	/* Set (or don't set) a timer handle to simulate single/burst/automatic weapon fire */
	void StartSimulateWeaponFire();

	/* Clear timer handle and stop weapon fire */
	void StopSimulateWeaponFire();

	/* Player called event to start weapon fire */
	void StartWeaponFire();

	/* Player called event to stop weapon fire */
	void StopWeaponFire();

	/** Gets called when the Character calls the Use function */
	void OnUsed_Implementation(ACharacter* Character) override;

	/** Gets called when the UsableActor is in the charaters' sight */
	void StartFocusItem_Implementation() override;

	/** Gets called when the UsableActor leaves the characters' sight */
	void EndFocusItem_Implementation() override;

	/** Spawn actor on character model */
	void AttachToPawn();
};
