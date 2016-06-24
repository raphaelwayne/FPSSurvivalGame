 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "UsableItem.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AUsableItem : public AStaticMeshActor
{
	GENERATED_BODY()

		AUsableItem();
	
public:
	/** Gets called when the Character calls the Use function */
	UFUNCTION(BlueprintNativeEvent, Category = "Inventory")
	void OnUsed(ACharacter* Character);

	/** Gets called when the UsableActor is in the charaters' sight */
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void StartFocusItem();

	/** Gets called when the UsableActor leaves the characters' sight */
	UFUNCTION(BlueprintNativeEvent, Category = "Item")
	void EndFocusItem();
};