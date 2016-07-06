// Copyright 2016, raphaelwayne

#pragma once

#include "GameFramework/HUD.h"
#include "FPSHUD.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AFPSHUD : public AHUD
{
	GENERATED_BODY()

	AFPSHUD();

	/** Primary draw call for the HUD */
	virtual void DrawHUD() override;

private:
	/** Crosshair asset pointer */
	UTexture2D* CrosshairTex;
};
