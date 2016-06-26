// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

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
protected:
	/* Our item needs a texture to display in the inventory */
	UPROPERTY(EditAnywhere, Category = "InventoryItemProperties")
	UTexture2D* InventoryDisplay;

	/* Our item needs a name to be distinguishable from other items */
	UPROPERTY(EditAnywhere, Category = "InventoryItemProperties")
	FString ItemName;

/** Member-Functions */
public:
	/** Override the OnUsed function - use _Implementation because it's a BlueprintNativeEvent */
	//void OnUsed_Implementation(ACharacter* Character) override;
};
