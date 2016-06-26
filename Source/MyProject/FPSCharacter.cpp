// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FPSCharacter.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());

	FirstPersonMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("FirstPersonMesh"));
	FirstPersonMesh->SetupAttachment(FirstPersonCameraComponent);
	// Only the player will see the mesh
	FirstPersonMesh->SetOnlyOwnerSee(true);
	FirstPersonMesh->SetOwnerNoSee(false);
	FirstPersonMesh->bCastDynamicShadow = false;
	FirstPersonMesh->CastShadow = false;	
	// Disable collision for the first person mesh
	FirstPersonMesh->SetCollisionEnabled(ECollisionEnabled::NoCollision);

	// Everyone but the player will see the regular body mesh
	GetMesh()->SetOwnerNoSee(true);

	// Properties used for interacting with an UsableActor
	MaxTraceDistance = 400.f;
	bHasNewFocus = true;

	CharacterHealth = 1.f;
	StomachSpace = 1.f;
	CharacterHunger = 0.5f;
	CharacterThirst = 0.7f;

	CharacterInventory.SetNum(MaxInventorySlots);
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFPSCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (Controller)
	{
		AUsableItem* UsableInView = GetUsableInView();
		// End focus
		if (FocusedUsableActor != UsableInView)
		{
			if (FocusedUsableActor)
			{
				FocusedUsableActor->EndFocusItem();
			}

			bHasNewFocus = true;
		}

		// Assign new focus
		FocusedUsableActor = UsableInView;

		// Start new focus
		if (UsableInView)
		{
			if (bHasNewFocus)
			{
				UsableInView->StartFocusItem();
				bHasNewFocus = false;
			}
		}
	}
}

// Called to bind functionality to input
void AFPSCharacter::SetupPlayerInputComponent(class UInputComponent* InputComponent)
{
	Super::SetupPlayerInputComponent(InputComponent);

	// Movement
	InputComponent->BindAxis("MoveForward", this, &AFPSCharacter::MoveForward);
	InputComponent->BindAxis("MoveRight", this, &AFPSCharacter::MoveRight);
	InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAction("Jump", IE_Pressed, this, &AFPSCharacter::OnStartJump);
	InputComponent->BindAction("Jump", IE_Released, this, &AFPSCharacter::OnStopJump);

	// Gameplay
	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnFire);
	InputComponent->BindAction("Use", IE_Pressed, this, &AFPSCharacter::PickupItem);
}

AUsableItem* AFPSCharacter::GetUsableInView()
{
	FVector CameraLocation;
	FRotator CameraRotation;
	// True if trace hit sth, if not false
	bool bTraceFound;

	// If we have no controller, we don't want our game to crash
	if (Controller == nullptr)
	{
		return nullptr;
	}

	// Returns the players point of view (mostly where the crosshair is located)
	GetController()->GetPlayerViewPoint(CameraLocation, CameraRotation);
	const FVector StartTrace = CameraLocation;
	const FVector DirectionTrace = CameraRotation.Vector();
	// Ends where we put our MaxTraceDistance to
	const FVector EndTrace = StartTrace + (DirectionTrace * MaxTraceDistance);

	FCollisionQueryParams TraceParams(FName(TEXT("UsableActor_Trace")), true, this);
	TraceParams.bTraceAsyncScene = true;
	TraceParams.bReturnPhysicalMaterial = true;
	TraceParams.bTraceComplex = true;

	FHitResult Hit(ForceInit);
	if (GetWorld())
	{
		// Returns true if the trace found somethign
		bTraceFound = GetWorld()->LineTraceSingleByChannel(Hit, StartTrace, EndTrace, ECC_GameTraceChannel1, TraceParams);
		if (bTraceFound)
		{
			//print("Trace hit: " + Hit.GetActor()->GetName());
		}
	}

	return Cast<AUsableItem>(Hit.GetActor());
}

void AFPSCharacter::MoveForward(float Val)
{
	if (Controller && Val  != 0.f)
	{
		// Here we find which way is forward
		FRotator Rotation = Controller->GetControlRotation();
		// Limit pitch when walking of falling
		if (GetCharacterMovement()->IsMovingOnGround() || GetCharacterMovement()->IsFalling())
		{
			Rotation.Pitch = 0.f;
		}
		// Move into that direction
		const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::X);
		AddMovementInput(Direction, Val);
	}
}

void AFPSCharacter::MoveRight(float Val)
{
	// Here we find which way is right
	FRotator Rotation = Controller->GetControlRotation();
	// Move into that direction
	const FVector Direction = FRotationMatrix(Rotation).GetScaledAxis(EAxis::Y);
	AddMovementInput(Direction, Val);
}

void AFPSCharacter::OnStartJump()
{
	bPressedJump = true;
}

void AFPSCharacter::OnStopJump()
{
	bPressedJump = false;
}

void AFPSCharacter::OnFire()
{
	// Check if there is a projectile to shoot
	if (ProjectileClass != NULL)
	{
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
		UWorld* const World = GetWorld();
		if (World)
		{
			FActorSpawnParameters SpawnParams;
			SpawnParams.Owner = this;
			SpawnParams.Instigator = Instigator;
			// Spawn the projectile at the muzzle
			AFPSProjectile* const Projectile = World->SpawnActor<AFPSProjectile>(ProjectileClass, MuzzleLocation, MuzzleRotation, SpawnParams);
			
			// Check if projectile really succeeded spawning
			if (Projectile != nullptr)
			{
				// Find the launch direction
				FVector const LaunchDir = MuzzleRotation.Vector();
				Projectile->InitVelocity(LaunchDir);
			}
		}
	}
}

void AFPSCharacter::PickupItem()
{
	AUsableItem* UsableInView = GetUsableInView();
	if (UsableInView)
	{
		UsableInView->OnUsed(this);
	}
}