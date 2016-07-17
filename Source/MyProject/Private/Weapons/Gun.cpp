// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Gun.h"
#include "FPSProjectile.h"
#include "FPSCharacter.h"

AGun::AGun()
{
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh1P"));
	// Only the player will see the mesh
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetOwnerNoSee(false);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	// Disable collision for the first person mesh
	Mesh1P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	RootComponent = Mesh1P;

	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh3P"));
	Mesh3P->CastShadow = true;
	Mesh3P->SetupAttachment(Mesh1P);

	CurrentState = EWeaponState::Idle;
	CurrentMode = EFireMode::Automatic;
}

EFireMode AGun::GetCurrentMode() const
{
	return CurrentMode;
}

EWeaponState AGun::GetCurrentState() const
{
	return CurrentState;
}

void AGun::SetOwningPlayer(AFPSCharacter* MyCharacter)
{
	OwningPawn = MyCharacter;
}

AFPSCharacter* AGun::GetOwningPlayer() const
{
	return OwningPawn;
}

void AGun::StartWeaponFire()
{
	// Check if there is a projectile to shoot
	if (Projectile)
	{
		bWantsToFire = true;
		// Get the camera transformations
		FVector CameraLoc;
		FRotator CameraRot;
		GetActorEyesViewPoint(CameraLoc, CameraRot);
		// MuzzleOffset is in camera space, so transform it to world space before offsetting 
		// from the camera to find the final muzzle position
		FVector const MuzzleLocation = CameraLoc + FTransform(CameraRot).TransformVector(MuzzleOffset);
		FRotator MuzzleRotation = CameraRot;
		// Skew the aim a bit upwards
		MuzzleRotation.Pitch += 10.f;
		UWorld* const World = OwningPawn->GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			// Spawn the projectile at the muzzle
			AFPSProjectile* const GunProjectile = World->SpawnActor<class AFPSProjectile>(Projectile, MuzzleLocation, MuzzleRotation, SpawnParams);
			// Check if projectile really succeeded spawning
			if (Projectile)
			{
				// Find the launch direction
				FVector const LaunchDir = MuzzleRotation.Vector();
				GunProjectile->InitVelocity(LaunchDir);
			}
		}

		// try and play the sound if specified
		if (WeaponSounds.FireSingleSound1P)
		{
			UGameplayStatics::PlaySoundAtLocation(this, WeaponSounds.FireSingleSound1P, OwningPawn->GetActorLocation());
		}

		//// try and play a firing animation if specified
		//if (WeaponAnimations.Animation1P)
		//{
		//	// Get the animation object for the arms mesh
		//	UAnimInstance* AnimInstance = FirstPersonMesh->GetAnimInstance();
		//	if (AnimInstance != NULL)
		//	{
		//		AnimInstance->Montage_Play(WeaponAnimations.Animation1P, 1.f);
		//	}
		//}
	}
}

void AGun::StopWeaponFire()
{
	bWantsToFire = false;
}

void AGun::OnUsed_Implementation(ACharacter* Character)
{
	UE_LOG(Guns, Log, TEXT("Weapon %s got picked up!"), *GetName());
}

void AGun::StartFocusItem_Implementation()
{
	Mesh3P->SetRenderCustomDepth(true);
}

void AGun::EndFocusItem_Implementation()
{
	Mesh3P->SetRenderCustomDepth(false);
}
