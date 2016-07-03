// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyPlayerController.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryHUD)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryHUD);
		InventoryWidget->AddToViewport();
		// Inventory should not be hidden at start
		InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
	}
}

void AMyPlayerController::ToggleInventoryVisibility()
{
	if (InventoryWidget)
	{
		if (bIsInventoryShowing)
		{
			// Inventory is showing
			InventoryWidget->SetVisibility(ESlateVisibility::Hidden);
		}
		else 
		{
			// Inventory is hidden
			InventoryWidget->SetVisibility(ESlateVisibility::Visible);
		}
		// Toggle between the visibility states of the inventory HUD
		bIsInventoryShowing = !bIsInventoryShowing;
	}
}

void AMyPlayerController::AddItemToInventory()
{
	InventoryWidget->OnInventoryChanged();
}

void AMyPlayerController::SetupInputComponent()
{
	Super::SetupInputComponent();

	InputComponent->BindAction("ToggleInventory", IE_Pressed, this, &AMyPlayerController::ToggleInventoryVisibility);
}
