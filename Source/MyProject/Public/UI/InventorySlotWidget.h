// Copyright 2016, raphaelwayne

#pragma once

#include "InventoryItem.h"
#include "Blueprint/UserWidget.h"
#include "InventorySlotWidget.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UInventorySlotWidget : public UUserWidget
{
	GENERATED_BODY()

/* Member-Variables */
public:
	/* Holds the reference to the displayed item texture */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* ItemIcon;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Functions")
	int32 SlotIndex;

/** Member-Functions */
	UFUNCTION(BlueprintCallable, Category = "Inventory Functions")
	void SetItemTexture(AInventoryItem* Item);
};
