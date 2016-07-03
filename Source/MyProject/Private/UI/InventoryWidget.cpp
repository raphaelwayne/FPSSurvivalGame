// Fill out your copyright notice in the Description page of Project Settings.

#include "MyProject.h"
#include "InventoryWidget.h"

void UInventoryWidget::MarkInventoryChanged()
{
	bIsInventoryChanged = true;
}
