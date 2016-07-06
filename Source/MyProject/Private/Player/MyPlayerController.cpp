// Copyright 2016, raphaelwayne

#include "MyProject.h"
#include "MyPlayerController.h"
#include "FPSCharacter.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryHUD)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryHUD);
		InventoryWidget->AddToViewport();
		// Inventory should not be hidden at start
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		InventoryWidget->ActionMenuVisibility = ESlateVisibility::Hidden;
	}
}

void AMyPlayerController::ToggleInventoryVisibility()
{
	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(GetCharacter());

	InventoryWidget->ToggleInventoryVisibility(bIsInventoryShowing);

	if (bIsInventoryShowing)
	{
		bShowMouseCursor = false;
		bEnableClickEvents = false;
		bEnableMouseOverEvents = false;
		MyCharacter->bCameraMovementEnabled = true;
		SetInputMode(InputGameMode);
	}
	else
	{
		bShowMouseCursor = true;
		bEnableClickEvents = true;
		bEnableMouseOverEvents = true;
		MyCharacter->bCameraMovementEnabled = false;
		InventoryWidget->OnInventoryOpened();
		SetInputMode(UiGameInputMode);
	}
	// Toggle between the visibility states of the inventory HUD
	bIsInventoryShowing = !bIsInventoryShowing;
}

void AMyPlayerController::RighMouseButtonClicked()
{
}

void AMyPlayerController::AddItemToInventory()
{
	InventoryWidget->OnInventoryChanged();
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AMyPlayerController::ToggleInventoryVisibility);
	InputComponent->BindAction("RightMouseButtonClicked", IE_Pressed, this, &AMyPlayerController::RighMouseButtonClicked);
}
