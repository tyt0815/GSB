// Fill out your copyright notice in the Description page of Project Settings.


#include "HUDs/GSBWindowWidget.h"
#include "HUDs/GSBOverlay.h" 
#include "HUDs/GSBWindowBody.h"
#include "HUDs/GSBWindowHead.h"
#include "Components/Button.h"
#include "Components/NamedSlot.h"
#include "Components/HorizontalBox.h"
#include "Components/HorizontalBoxSlot.h"

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
		GSBOverlay->CloseWindow_Internal(this);
	}
	else
	{
		OnCloseWindow();
	}
}

void UGSBWindowWidget::OnOpenWindow(UGSBOverlay* InGSBOverlay, UObject* InTargetObject)
{
	GSBOverlay = InGSBOverlay;
	SetTargetObject(InTargetObject);
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

void UGSBWindowWidget::ClearWindowHead()
{
	WindowHeadSlot->ClearChildren();
}

void UGSBWindowWidget::AttachWindowHead(UGSBWindowHead* WindowHead)
{
	if (IsValid(WindowHead))
	{
		if (UHorizontalBoxSlot* HorizontalBoxSlot = Cast<UHorizontalBoxSlot>(WindowHeadSlot->AddChild(WindowHead)))
		{
			HorizontalBoxSlot->SetHorizontalAlignment(EHorizontalAlignment::HAlign_Center);
			HorizontalBoxSlot->SetVerticalAlignment(EVerticalAlignment::VAlign_Center);
		}
		WindowHead->OnAttachedToWindow(this);
	}
}

UWidget* UGSBWindowWidget::GetContentWidget() const
{
	return WindowBodySlot->GetChildAt(0);
}