// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "MyPlayerController.h"

void AMyPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (InventoryHUD)
	{
		InventoryWidget = CreateWidget<UInventoryWidget>(this, InventoryHUD);
	}
}

void AMyPlayerController::ToggleInventoryVisibility()
{
	AFPSCharacter* MyCharacter = Cast<AFPSCharacter>(GetPawn());
	if (InventoryWidget)
	{
		if (bIsInventoryShowing)
		{
			// Inventory is showing
			bIsInventoryShowing = false;
			InventoryWidget->RemoveFromViewport();
		}
		else 
		{
			// Inventory is hidden
			bIsInventoryShowing = true;
			InventoryWidget->ItemsArray = MyCharacter->GetInventory();
			InventoryWidget->Show();
		}
	}
}
