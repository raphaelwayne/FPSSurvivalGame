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
public:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	TArray<AInventoryItem*> ItemsArray;

	/** Member-Functions */
public:
	/*Adds the widget into the viewport and populates the inventory slots*/
	UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	void Show();

	/** Removes the widget from the viewport */
	//UFUNCTION(BlueprintImplementableEvent, Category = "UI")
	//void Hide();
};
