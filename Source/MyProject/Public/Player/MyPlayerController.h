// Copyright 2016, raphaelwayne

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

	/* If true, input game actions are allowed */
	bool bAllowGameActions;

private:
	/* Controller and UI can control input */
	FInputModeGameOnly InputGameMode;

	/* UI can control input */
	FInputModeGameAndUI UiGameInputMode;

	/* Determine if inventorytoggle should show or hide the widget */
	bool bIsInventoryShowing;

	/** Member-Functions */
public:
	AMyPlayerController();

	/** We have to override the BeginPlay function to implement own behaviour */
	void BeginPlay() override;

	/** Toggles the inventorywidget on demand of the player */
	void ToggleInventoryVisibility();

	/** Right mouse button click event */
	void RighMouseButtonClicked();

	/** Add the picked up item to the right slot */
	void AddItemToInventory();

	void SetupInputComponent() override;

	/** Returns true when player is allowed to do something game input actions related */
	bool IsGameActionAllowed();
};
