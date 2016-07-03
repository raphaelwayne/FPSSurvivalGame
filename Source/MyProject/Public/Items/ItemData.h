// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "ItemData.generated.h"

USTRUCT(BlueprintType)
struct FItemDisplayProperties
{
	GENERATED_USTRUCT_BODY()

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDisplayData")
	FString ItemName;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDisplayData")
	UTexture2D* DisplayTexture;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDisplayData")
	FString ItemDescription;

	/* Position of the item in the inventory */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "ItemDisplayData")
	int32 SlotIndex;

	FItemDisplayProperties()
	{
		ItemName = TEXT("Default");
		DisplayTexture = nullptr;
		ItemDescription = TEXT("Description");
		SlotIndex = -1;
	}

	FItemDisplayProperties(FString Name, UTexture2D* Texture, FString Description)
	{
		ItemName = Name;
		DisplayTexture = Texture;
		ItemDescription = Description;
		SlotIndex = -1;
	}
};

