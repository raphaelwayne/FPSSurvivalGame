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

	WeaponProperties.FireRate = 0.f;
	WeaponProperties.ClipAmmo = 30.f;
	WeaponProperties.InitialClips = 0.f;
	WeaponProperties.MaxAmmo = 30.f;
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

void AGun::StartSimulateWeaponFire()
{
	// Attach timer handle to world timer
	bWantsToFire = true;
	CurrentState = EWeaponState::Firing;
	GetWorldTimerManager().SetTimer(TimerHandle_HandleWeaponFire, this, &AGun::StartWeaponFire, WeaponProperties.FireRate, false);
}

void AGun::StopSimulateWeaponFire()
{
	// Clear the timer
	bWantsToFire = false;
	CurrentState = EWeaponState::Idle;
	GetWorldTimerManager().ClearTimer(TimerHandle_HandleWeaponFire);
}

void AGun::StartWeaponFire()
{
	// Check if there is a projectile to shoot
	if (OwningPawn && Projectile && bWantsToFire)
	{
		// Get the camera transformations
		FVector CameraLoc;
		FRotator CameraRot;
		GetOwningPawn()->GetActorEyesViewPoint(CameraLoc, CameraRot);
		// MuzzleOffset is in camera space, so transform it to world space before offsetting 
		// from the camera to find the final muzzle position
		MuzzleLocation = Mesh3P->GetSocketLocation("Muzzle");
		FRotator MuzzleRotation = CameraRot;
		// Skew the aim a bit upwards
		MuzzleRotation.Pitch += 1.f;
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
		if (WeaponAnimations.Animation1P)
		{
			// Get the animation object for the arms mesh
			AFPSCharacter* Player = Cast<AFPSCharacter>(GetOwningPawn());
			USkeletalMeshComponent* PawnMesh = Player->Get1PMesh();
			UAnimInstance* AnimInstance = PawnMesh->GetAnimInstance();
			if (AnimInstance)
			{
				AnimInstance->Montage_Play(WeaponAnimations.Animation1P, 1.f);
			}
		}

		if (CurrentState == EWeaponState::Firing && WeaponProperties.FireRate > 0.f)
		{
			GetWorldTimerManager().SetTimer(TimerHandle_HandleWeaponFire, this, &AGun::StartWeaponFire, WeaponProperties.FireRate, false);
		}
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
				bIsEquipped = true;
			}
		}
	}
}
