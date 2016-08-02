// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Gun.h"
#include "FPSProjectile.h"
#include "FPSCharacter.h"

AGun::AGun()
{
	Mesh3P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("WeaponMesh3P"));
	Mesh3P->CastShadow = true;
	Mesh3P->SetCollisionEnabled(ECollisionEnabled::QueryAndPhysics);
	Mesh3P->SetSimulatePhysics(true);
	RootComponent = Mesh3P;

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

void AGun::SetOwningPawn(ACharacter* Character)
{
	OwningPawn = Character;
}

ACharacter* AGun::GetOwningPawn() const
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
	if (Character)
	{
		UE_LOG(GunLog, Log, TEXT("Weapon %s got picked up!"), *GetName());
		Mesh3P->CastShadow = false;
		Mesh3P->SetCollisionEnabled(ECollisionEnabled::NoCollision);
		Mesh3P->SetSimulatePhysics(false);
		AttachToPawn();
	}
}

void AGun::StartFocusItem_Implementation()
{
	Mesh3P->SetRenderCustomDepth(true);
}

void AGun::EndFocusItem_Implementation()
{
	Mesh3P->SetRenderCustomDepth(false);
}

void AGun::AttachToPawn()
{
	if (GetOwningPawn())
	{
		// Need to cast to AFPSCharacter to get a first person mesh
		AFPSCharacter* Player = Cast<AFPSCharacter>(GetOwningPawn());
		USkeletalMeshComponent* PawnMesh = Player->Get1PMesh();
		if (PawnMesh)
		{
			//Mesh3P->AttachToComponent(PawnMesh, FAttachmentTransformRules::KeepWorldTransform, "GripPoint");
			if (Mesh3P->AttachToComponent(PawnMesh, FAttachmentTransformRules(EAttachmentRule::SnapToTarget, true), TEXT("GripPoint")))
			{
				UE_LOG(GunLog, Log, TEXT("Gun %s got successfully attached."), *GetName());
			}
		}
	}
}
