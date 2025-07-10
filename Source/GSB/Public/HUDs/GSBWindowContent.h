// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWidget.h"
#include "GSBWindowContent.generated.h"

class UGSBWindowWidget;


UCLASS()
class GSB_API UGSBWindowContent : public UGSBWidget
{
	GENERATED_BODY()
public:
	UFUNCTION()
	void CloseWindow();

	bool IsLostFocus(const FWidgetPath& NewWidgetPath);

	virtual void OnAttachedToWindow(UGSBWindowWidget* Window);

protected:
	UGSBWindowWidget* ParentWindow;
};
