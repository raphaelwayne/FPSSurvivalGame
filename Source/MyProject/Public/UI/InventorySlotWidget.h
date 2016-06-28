// Fill out your copyright notice in the Description page of Project Settings.

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
protected:
	/* Holds the reference to the displayed item texture */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UTexture2D* ItemIcon;

	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetEquippedItem();

/** Member-Functions */
public:
	UFUNCTION(BlueprintCallable, Category = "UI")
	void SetItemTexture(AInventoryItem* Item);
};
