// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
public:
	bool bIsInventoryChanged;

	UPROPERTY(BlueprintReadWrite)
	ESlateVisibility ActionMenuVisibility;

	UPROPERTY(BlueprintReadWrite)
	bool bIsInventoryActive;

	/** Member-Functions */
	/** Gets called when something in the inventory changed. Updates the grid with icons, etc.  */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Events")
	void OnInventoryChanged();

	/** Gets called when the inventory is open */
	UFUNCTION(BlueprintImplementableEvent, Category = "Inventory Events")
	void OnInventoryOpened();

	void MarkInventoryChanged();

	void ToggleInventoryVisibility(bool bIsVisible);
};
