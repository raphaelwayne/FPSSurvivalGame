// Copyright 2016, raphaelwayne

#include "MyProject.h"
#include "FPSCharacter.h"
#include "InventoryItem.h"
#include "InventoryComponent.h"
#include "MyPlayerController.h"
#include "Gun.h"

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
	fMouseSensivity = 80.0f;

	bCameraMovementEnabled = true;
	bWantsToFire = false;
}

// Called when the game starts or when spawned
void AFPSCharacter::BeginPlay()
{
	Super::BeginPlay();
	if (CurrentWeapon)
	{
		CurrentWeapon->SetOwningPlayer(this);
	}
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
	InputComponent->BindAction("Fire", IE_Pressed, this, &AFPSCharacter::OnStartFire);
	InputComponent->BindAction("Fire", IE_Released, this, &AFPSCharacter::OnStopFire);
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

void AFPSCharacter::OnStartFire()
{
	AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
	if (MyPlayerController && MyPlayerController->IsGameActionAllowed())
	{
		bWantsToFire = true;
		if (CurrentWeapon && bWantsToFire)
		{
			CurrentWeapon->StartWeaponFire();
		}
	}
}

void AFPSCharacter::OnStopFire()
{
	bWantsToFire = false;
	if (CurrentWeapon && !bWantsToFire)
	{
		CurrentWeapon->StopWeaponFire();
	}
}

bool AFPSCharacter::HasWeaponEquipped()
{
	return CurrentWeapon != nullptr;
}

void AFPSCharacter::PickupItem()
{
	AUsableItem* UsableInView = Cast<AUsableItem>(GetUsableInView());

	if (UsableInView != nullptr)
	{
		// The item is a inventory item
		if (UsableInView->IsA(AInventoryItem::StaticClass()))
		{
			AInventoryItem* ItemInView = Cast<AInventoryItem>(UsableInView);
			AMyPlayerController* MyPlayerController = Cast<AMyPlayerController>(UGameplayStatics::GetPlayerController(GetWorld(), 0));
			int32 FreeSlot = Inventory->GetItems().Find(nullptr);
			if (FreeSlot != INDEX_NONE)
			{
				Inventory->AddItem(ItemInView);
				MyPlayerController->AddItemToInventory();
				ItemInView->OnUsed(this);
			}
			else
			{
				UE_LOG(ItemLog, Error, TEXT("Cannot pick up more than %d Items!"), Inventory->MaxInventorySlots);
			}
		}

		// The item is a gun
		if (UsableInView->IsA(AGun::StaticClass()))
		{
			// check if character has a equipped gun
			if (HasWeaponEquipped())
			{
				UE_LOG(GunLog, Log, TEXT("Weapon %s already equipped."), *CurrentWeapon->GetName());
			}
			else
			{
				AGun* WeaponInView = Cast<AGun>(UsableInView);
				CurrentWeapon = WeaponInView;
				UE_LOG(GunLog, Log, TEXT("Character has picked up %s"), *CurrentWeapon->GetName());
			}
		}
	}
}

bool AFPSCharacter::IsFiring() const
{
	return bWantsToFire;
}
