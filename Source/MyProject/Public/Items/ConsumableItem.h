// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Items/InventoryItem.h"
#include "ConsumableItem.generated.h"

/**
 * 
 */

UCLASS()
class MYPROJECT_API AConsumableItem : public AInventoryItem
{
	GENERATED_BODY()
	
public:
	AConsumableItem();
	
	// Event to call when consuming
	UFUNCTION(BlueprintImplementableEvent, BlueprintCallable, Category = "Consumable Events")
	void OnConsumed(ACharacter* Invoker);
};
