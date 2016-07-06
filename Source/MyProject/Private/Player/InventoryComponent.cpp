// Copyright 2016, raphaelwayne

#include "MyProject.h"
#include "InventoryComponent.h"

UInventoryComponent::UInventoryComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	bWantsBeginPlay = false;
	PrimaryComponentTick.bCanEverTick = false;

	MaxInventorySlots = 4;
	Items.SetNum(MaxInventorySlots);
}

int32 UInventoryComponent::AddItem(AInventoryItem* Item)
{
	int32 FreeSlot = GetEmptySlot();

	// No empty slot was found or the index is out of range
	if (FreeSlot == -1)
	{
		UE_LOG(LogTemp, Error, TEXT("No available slot found!"));
		return -1;
	}

	Items[FreeSlot] = Item;
	Item->ItemProperties.SlotIndex = FreeSlot;
	UE_LOG(LogTemp, Warning, TEXT("Item %s was picked up."), *Items[FreeSlot]->GetItemProperties().ItemName);
	return Item->GetItemProperties().SlotIndex;
}

int32 UInventoryComponent::GetEmptySlot()
{
	int32 FreeSlot = Items.Find(nullptr);

	// Is there no available slot ?
	if (FreeSlot == INDEX_NONE)
	{
		UE_LOG(LogTemp, Warning, TEXT("No empty inventory slot found!"));
		return -1;
	}
		
	return FreeSlot;
}

FItemDisplayProperties UInventoryComponent::GetDataFromSlot(int32 ItemSlot)
{
	// Is the slot out of range or is there simply none available ?
	if (ItemSlot > Items.Num() || ItemSlot == -1)
	{
		UE_LOG(LogTemp, Error, TEXT("Item is not in Inventory!"));
		FItemDisplayProperties FDefaultItemProperties;
		return FDefaultItemProperties;
	}
	
	return Items[ItemSlot]->GetItemProperties();
}
