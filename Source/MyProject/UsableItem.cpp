// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UsableItem.h"

#define print(DebugString) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, DebugString)

AUsableItem::AUsableItem()
{
	// Shadow will go away when object is destroyed
	SetMobility(EComponentMobility::Movable);
}

void AUsableItem::OnUsed_Implementation(ACharacter* Character)
{
	GetStaticMeshComponent()->DestroyComponent();
	print(GetName() + " destroyed!");
}

void AUsableItem::StartFocusItem_Implementation()
{
	// Used by PostProcess to enable outlines
	GetStaticMeshComponent()->SetRenderCustomDepth(true);
}

void AUsableItem::EndFocusItem_Implementation()
{
	// Used by PostProcess to enable outlines
	GetStaticMeshComponent()->SetRenderCustomDepth(false);
}
