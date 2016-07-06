// Copyright 2016, raphaelwayne

#include "MyProject.h"
#include "InventoryWidget.h"

void UInventoryWidget::MarkInventoryChanged()
{
	bIsInventoryChanged = true;
}

void UInventoryWidget::ToggleInventoryVisibility(bool bIsVisible)
{
	if (bIsVisible)
	{
		// Inventory is showing
		SetVisibility(ESlateVisibility::Hidden);
		bIsInventoryActive = false;
	}
	else 
	{
		// Inventory is hidden
		SetVisibility(ESlateVisibility::Visible);
		bIsInventoryActive = true;
	}
}
