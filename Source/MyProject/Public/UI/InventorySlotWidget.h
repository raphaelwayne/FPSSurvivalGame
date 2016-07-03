// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Inventory Slot")
	int32 SlotIndex;

/** Member-Functions */
	UFUNCTION(BlueprintCallable, Category = "Inventory Slot")
	void SetItemTexture(UTexture2D* ItemTexture);
};
