// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBWindow.h"
#include "HUDs/GSBWindowFrame.h"
#include "Components/Button.h"

void UGSBWindow::NativeConstruct()
{
	Super::NativeConstruct();

	WindowFrame->CloseButton->OnClicked.AddDynamic(this, &UGSBWindow::Close);
}

void UGSBWindow::Close()
{
	RemoveFromParent();
}
