// Copyright 2016, raphaelwayne

#include "MyProject.h"
#include "InventoryItem.h"

AInventoryItem::AInventoryItem()
{
	// Create the static mesh component
	PickupMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PickupMesh"));
	RootComponent = PickupMesh;
}

void AInventoryItem::OnUsed_Implementation(ACharacter* Character)
{
	UE_LOG(ItemLog, Log, TEXT("Item %s got picked up!"), *GetName());
	PickupMesh->DestroyComponent();
}

void AInventoryItem::StartFocusItem_Implementation()
{
	PickupMesh->SetRenderCustomDepth(true);
}

void AInventoryItem::EndFocusItem_Implementation()
{
	PickupMesh->SetRenderCustomDepth(false);
}