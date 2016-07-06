// Copyright 2016, raphaelwayne

#include "MyProject.h"
#include "MyPlayerController.h"
#include "FPSHUD.h"
#include "FPSGameMode.h"
#include "Engine.h"
#include "Blueprint/UserWidget.h"

AFPSGameMode::AFPSGameMode()
{
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnObject(TEXT("/Game/Blueprints/BP_FPSCharacter"));
	if(PlayerPawnObject.Class != NULL)
	{
		DefaultPawnClass = PlayerPawnObject.Class;
	}

	HUDClass = AFPSHUD::StaticClass();
	PlayerControllerClass = AMyPlayerController::StaticClass();
}

void AFPSGameMode::BeginPlay()
{
	// Call the parent's logic of BeginPlay, so it doesn't get left behind
	Super::BeginPlay();
	StartMatch();

	if (HUDWidgetClass != nullptr)
	{
		CurrentWidget = CreateWidget<UUserWidget>(GetWorld(), HUDWidgetClass);
		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
}


