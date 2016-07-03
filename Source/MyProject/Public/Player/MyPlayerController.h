// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "InventoryWidget.h"
#include "GameFramework/PlayerController.h"
#include "MyPlayerController.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMyPlayerController : public APlayerController
{
	GENERATED_BODY()
	
	
	/* Member-Variables */
protected:
	/** The widget class to use for the HUD */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "UI")
	TSubclassOf<class UInventoryWidget> InventoryHUD;

	/** Instance of the HUD */
	UPROPERTY()
	class UInventoryWidget* InventoryWidget;

private:
	/* Determine if inventorytoggle should show or hide the widget */
	bool bIsInventoryShowing;

	/** Member-Functions */
public:
	/** We have to override the BeginPlay function to implement own behaviour */
	void BeginPlay() override;

	/** Toggles the inventorywidget on demand of the player */
	void ToggleInventoryVisibility();

	/** Add the picked up item to the right slot */
	void AddItemToInventory();

	void SetupInputComponent() override;
};
