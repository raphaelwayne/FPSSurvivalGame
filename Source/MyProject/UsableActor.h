 //Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine/StaticMeshActor.h"
#include "UsableActor.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AUsableActor : public AStaticMeshActor
{
	GENERATED_BODY()

	AUsableActor();
	
public:
	/** Gets called when the Character calls the Use function */
	virtual void OnUsed(ACharacter* Character);

	/** Gets called when the UsableActor is in the charaters' sight */
	virtual void StartFocusItem();

	/** Gets called when the UsableActor leaves the characters' sight */
	virtual void EndFocusItem();
};
