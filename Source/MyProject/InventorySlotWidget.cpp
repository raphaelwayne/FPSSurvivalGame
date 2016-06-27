// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "InventorySlotWidget.h"

void UInventorySlotWidget::SetEquippedItem()
{
}

void UInventorySlotWidget::SetItemTexture(AInventoryItem * Item)
{
	// If the item is a valid one, update it's texture. If the item is not valid, make sure no wrong information is shown
	Item ? ItemIcon = Item->GetInventoryDisplayTexture() : ItemIcon = nullptr;
}
