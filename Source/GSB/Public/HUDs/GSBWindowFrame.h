// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBWindowFrame.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBWindowFrame : public UUserWidget
{
	GENERATED_BODY()
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UNamedSlot* WindowHeadSlot;

	UPROPERTY(meta = (BindWidget))
	UNamedSlot* WindowBodySlot;

	friend class UGSBWindow;
};
