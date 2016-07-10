// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "Gun.h"

EFireMode AGun::GetCurrentMode() const
{
	return CurrentMode;
}

EWeaponState AGun::GetCurrentState() const
{
	return CurrentState;
}

AFPSCharacter* AGun::GetOwningPLayer() const
{
	return OwningPawn;
}
