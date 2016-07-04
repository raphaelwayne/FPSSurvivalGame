// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FPSCharacter.h"
#include "InventoryItem.h"
#include "InventoryComponent.h"
#include "MyPlayerController.h"

// Sets default values
AFPSCharacter::AFPSCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Add the camera
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());

	// Add the firstperson mesh
	// Note: the thirdperson mesh for others to see is set in the BP_FPSCharacter
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

	// Add the inventory component
	Inventory = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));
	Inventory->bAutoRegister = true;
	Inventory->bWantsInitializeComponent = true;
	Inventory->Activate();

	// Properties used for interacting with an UsableActor
	MaxTraceDistance = 400.f;
	bHasNewFocus = true;

	CharacterHealth = 1.f;
	StomachSpace = 1.f;
	CharacterHunger = 0.5f;
	CharacterThirst = 0.7f;

	//1 means normal mouse input
	n8MouseInvertion = 1;
	fMouseSensivity = 200.f;

	bCameraMovementEnabled = true;
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

	// We control every tick if the player has focused on a new object or left the old object
	if (Controller)
	{
		// Is there a UsableActor in the crosshair of a player ?
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
	//InputComponent->BindAxis("Turn", this, &APawn::AddControllerYawInput);
	//InputComponent->BindAxis("LookUp", this, &APawn::AddControllerPitchInput);
	InputComponent->BindAxis("Turn", this, &AFPSCharacter::CameraYaw);
	InputComponent->BindAxis("LookUp", this, &AFPSCharacter::CameraPitch);
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

void AFPSCharacter::CameraYaw(float fAmount)
{
	if (bCameraMovementEnabled)
		AddControllerYawInput(fMouseSensivity * fAmount * GetWorld()->GetDeltaSeconds());
	else
		AddControllerYawInput(0.f);
}

void AFPSCharacter::CameraPitch(float fAmount)
{
	if (bCameraMovementEnabled)
		AddControllerPitchInput(n8MouseInvertion * fMouseSensivity * fAmount * GetWorld()->GetDeltaSeconds());
	else 
		AddControllerPitchInput(0.f);
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
	AInventoryItem* ItemInView = Cast<AInventoryItem>(GetUsableInView());

	if (ItemInView != nullptr)
	{
		AMyPlayerController* OwningPawn = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
		int32 FreeSlot = Inventory->GetItems().Find(nullptr);
		if (FreeSlot != INDEX_NONE)
		{
			Inventory->AddItem(ItemInView);
			OwningPawn->AddItemToInventory();
			ItemInView->OnUsed(this);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Cannot pick up more than %d Items!"), Inventory->MaxInventorySlots);
		}
	}
}
