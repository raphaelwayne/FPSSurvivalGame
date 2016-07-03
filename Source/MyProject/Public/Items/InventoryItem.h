// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.h"
#include "UsableItem.h"
#include "InventoryItem.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AInventoryItem : public AUsableItem
{
	GENERATED_BODY()
	
/* Member-Variables */	
public:
	/* Neccessary item properties */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDisplayData")
	FItemDisplayProperties ItemProperties;

/** Member-Functions */
public:
	/** Returns item properties as struct */
	UFUNCTION()
	FItemDisplayProperties GetItemProperties() { return ItemProperties; }
};
