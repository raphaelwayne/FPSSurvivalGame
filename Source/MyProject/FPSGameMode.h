// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "GameFramework/GameMode.h"
#include "FPSGameMode.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AFPSGameMode : public AGameMode
{
	GENERATED_BODY()
	
public:
	AFPSGameMode();

	virtual void BeginPlay() override;
};
