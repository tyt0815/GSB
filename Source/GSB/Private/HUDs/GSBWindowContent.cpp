// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBWindowContent.h"
#include "HUDs/GSBWindowWidget.h"

void UGSBWindowContent::CloseWindow()
{
	check(ParentWindow);
	ParentWindow->CloseWindow();
}

bool UGSBWindowContent::IsLostFocus(const FWidgetPath& NewWidgetPath)
{
	return (IsValid(ParentWindow) && !NewWidgetPath.ContainsWidget(&ParentWindow->TakeWidget().Get())) ||
		!NewWidgetPath.ContainsWidget(&TakeWidget().Get());
}

void UGSBWindowContent::OnAttachedToWindow(UGSBWindowWidget* Window)
{
	ParentWindow = Window;
	SetTargetObject(ParentWindow->GetTargetObject());
}
