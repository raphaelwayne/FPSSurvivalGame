// Copyright 2016, raphaelwayne

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

	UPROPERTY(VisibleAnywhere, Category = "ItemMesh")
	UStaticMeshComponent* PickupMesh;

/** Member-Functions */
public:
	AInventoryItem();

	/** Returns item properties as struct */
	UFUNCTION()
	FItemDisplayProperties GetItemProperties() { return ItemProperties; }

	/** Gets called when the Character calls the Use function */
	void OnUsed_Implementation(ACharacter* Character) override;

	/** Gets called when the UsableActor is in the charaters' sight */
	void StartFocusItem_Implementation() override;

	/** Gets called when the UsableActor leaves the characters' sight */
	void EndFocusItem_Implementation() override;

	/** Return the mesh for the pickup */
	FORCEINLINE class UStaticMeshComponent* GetMesh() const { return PickupMesh; }
};
