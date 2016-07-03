// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InventoryItem.h"
#include "Blueprint/UserWidget.h"
#include "InventoryWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UInventoryWidget : public UUserWidget
{
	GENERATED_BODY()

	/* Member-Variables */
private:
	/* Marks the inventory as changed */
	bool bIsInventoryChanged;

	/** Member-Functions */
public:
	/** Gets called when something in the inventory changed. Updates the grid with icons, etc.  */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Events")
	void OnInventoryChanged();

	void MarkInventoryChanged();
};
