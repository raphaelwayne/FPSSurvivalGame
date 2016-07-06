// Copyright 2016, raphaelwayne

#include "MyProject.h"
#include "InventorySlotWidget.h"

void UInventorySlotWidget::SetItemTexture(AInventoryItem* Item)
{
	(Item != nullptr) ? ItemIcon = Item->GetItemProperties().DisplayTexture : ItemIcon = nullptr;
}