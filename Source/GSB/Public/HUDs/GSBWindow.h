// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBWindow.generated.h"

class UGSBWindowFrame;
class UGSBOverlayPanel;

UCLASS()
class GSB_API UGSBWindow : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	virtual void OnOpened(UGSBOverlayPanel* InParentOverlay);

	virtual void OnClosed();

	UFUNCTION()
	void Close();
	
private	:
	UPROPERTY(meta = (BindWidget))
	UGSBWindowFrame* WindowFrame;

	UGSBOverlayPanel* ParentOverlay;
};
