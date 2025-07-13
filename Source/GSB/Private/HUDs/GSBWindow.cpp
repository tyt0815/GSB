// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBWindow.h"
#include "HUDs/GSBWindowFrame.h"
#include "HUDs/GSBOverlay.h"
#include "Components/Button.h"

void UGSBWindow::NativeConstruct()
{
	Super::NativeConstruct();

	WindowFrame->CloseButton->OnClicked.AddDynamic(this, &UGSBWindow::Close);
	SetFocus();
}

void UGSBWindow::OnOpened(UGSBOverlay* InParentOverlay)
{
	ParentOverlay = InParentOverlay;
	
}

void UGSBWindow::OnClosed()
{
	RemoveFromParent();
}

void UGSBWindow::Close()
{
	ParentOverlay->CloseWindow(this);
}
