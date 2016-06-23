// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "UsableActor.h"

#define print(DebugString) if (GEngine) GEngine->AddOnScreenDebugMessage(-1, 1.5, FColor::White, DebugString)

AUsableActor::AUsableActor()
{
	// Shadow will go away when object is destroyed
	SetMobility(EComponentMobility::Movable);
}

void AUsableActor::OnUsed(ACharacter* Character)
{
	this->Destroy();
	print(this->GetName() + " destroyed!");
}

void AUsableActor::StartFocusItem()
{
	// Used by PostProcess to enable outlines
	this->GetStaticMeshComponent()->SetRenderCustomDepth(true);
}

void AUsableActor::EndFocusItem()
{
	// Used by PostProcess to enable outlines
	this->GetStaticMeshComponent()->SetRenderCustomDepth(false);
}
