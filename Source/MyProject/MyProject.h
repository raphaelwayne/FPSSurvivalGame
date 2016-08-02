// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "Engine.h"

// Logging for guns
DECLARE_LOG_CATEGORY_EXTERN(GunLog, Log, All);

// Logging for items
DECLARE_LOG_CATEGORY_EXTERN(ItemLog, Log, All);

// Custom Tracechannels
#define	COLLISION_USABLE	ECC_GameTraceChannel1