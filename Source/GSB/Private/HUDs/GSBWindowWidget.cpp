// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBWindowWidget.h"
#include "HUDs/GSBOverlay.h" 
#include "HUDs/GSBWindowBody.h"
#include "Components/Button.h"
#include "Components/NamedSlot.h"

void UGSBWindowWidget::NativeConstruct()
{
	Super::NativeConstruct();

	SetIsFocusable(true);
	CloseButton->OnClicked.AddDynamic(this, &UGSBWindowWidget::CloseWindow);
}

void UGSBWindowWidget::CloseWindow()
{
	if (GSBOverlay)
	{
		GSBOverlay->CloseWindow(this);
	}
	else
	{
		OnCloseWindow();
	}
}

void UGSBWindowWidget::OnOpenWindow(UGSBOverlay* InGSBOverlay)
{
	GSBOverlay = InGSBOverlay;
	SetFocus();
	SetPositionInViewport(FVector2D(1, 1));
}

void UGSBWindowWidget::OnCloseWindow()
{
	RemoveFromParent();
}

void UGSBWindowWidget::AttachWindowBody(UGSBWindowBody* InWindowBody)
{
	WindowBodySlot->ClearChildren();
	WindowBody = InWindowBody;
	if (IsValid(WindowBody))
	{
		WindowBodySlot->AddChild(WindowBody);
		WindowBody->OnAttachedToWindow(this);
	}
}

UWidget* UGSBWindowWidget::GetContentWidget() const
{
	return WindowBodySlot->GetChildAt(0);
}