// Copyright 2016, raphaelwayne

#include "MyProject.h"
#include "FPSHUD.h"


AFPSHUD::AFPSHUD()
{
	static ConstructorHelpers::FObjectFinder<UTexture2D> CrosshairTexObj(TEXT("Texture2D'/Game/Textures/FirstPersonCrosshair.FirstPersonCrosshair'"));
	CrosshairTex = CrosshairTexObj.Object;
}

void AFPSHUD::DrawHUD()
{
	Super::DrawHUD();
	
	// Draw very simple crosshair
	// Find center of the canvas
	const FVector2D Center(Canvas->ClipX * 0.5f, Canvas->ClipY * 0.5f);
	// Offset by half of the texture's dimension so that the center of the texture aligns
	// with the center of the canvas
	const FVector2D CrosshairDrawPosition(Center.X - (CrosshairTex->GetSurfaceWidth() * 0.5), Center.Y - (CrosshairTex->GetSurfaceHeight() * 0.5));
	// Draw the crosshair
	FCanvasTileItem TileItem(CrosshairDrawPosition, CrosshairTex->Resource, FLinearColor::White);
	TileItem.BlendMode = SE_BLEND_Translucent;
	Canvas->DrawItem(TileItem);
}
