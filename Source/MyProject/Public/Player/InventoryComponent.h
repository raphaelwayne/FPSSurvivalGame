// Copyright 2016, raphaelwayne

#pragma once

#include "ItemData.h"
#include "InventoryItem.h"
#include "Components/ActorComponent.h"
#include "InventoryComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class MYPROJECT_API UInventoryComponent : public UActorComponent
{
	GENERATED_BODY()

/* Member-Variables */
public:
	/* Items exist in the inventorycomponent */
	//UPROPERTY(VisibleAnywhere)
	TArray<AInventoryItem*> Items;

	/* Max amount of items the player can carry */
	int32 MaxInventorySlots;

/** Member-Function */
public:	
	/** Sets default values for this component's properties */
	UInventoryComponent();

	/** Adds the item to the inventory array and returns it's slot index */
	int32 AddItem(AInventoryItem* Item);

	/** Scans the inventory for an empty slot and return the index of that slot */
	int32 GetEmptySlot();

	/** Returns itemdisplaydata at specific slot */
	UFUNCTION(BlueprintCallable, Category = "ItemDisplayData")
	FItemDisplayProperties GetDataFromSlot(int32 ItemSlot);

	UFUNCTION(BlueprintPure, Category = "Inventory")
	TArray<AInventoryItem*> GetItems() { return Items; }

	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void SetToNullptr(int32 SlotIndex);

	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void DropItem(int32 SlotIndex);
};
