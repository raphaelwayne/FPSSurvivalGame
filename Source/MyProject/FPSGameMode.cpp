// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "FPSHUD.h"
#include "FPSGameMode.h"
#include "Engine.h"

AFPSGameMode::AFPSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("/Game/Blueprints/BP_FPSCharacter"));
	if(PlayerPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}

	HUDClass = AFPSHUD::StaticClass();
}

void AFPSGameMode::BeginPlay()
{
	// Call the parent's logic of BeginPlay, so it doesn't get left behind
	Super::BeginPlay();
	StartMatch();
}


