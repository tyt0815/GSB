// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBWindowBody.generated.h"

class UGSBWindowWidget;

UCLASS()
class GSB_API UGSBWindowBody : public UUserWidget
{
	GENERATED_BODY()
	
protected:
	virtual void OnAttachedToWindow(UGSBWindowWidget* Window);

	UGSBWindowWidget* ParentWindow;

	friend UGSBWindowWidget;
};
