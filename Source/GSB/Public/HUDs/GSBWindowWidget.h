// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBWindowWidget.generated.h"

class UButton;
class UGSBOverlay;
class UGSBWindowBody;

UCLASS()
class GSB_API UGSBWindowWidget : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void CloseWindow();

	void OnOpenWindow(UGSBOverlay* InGSBOverlay);

	void OnCloseWindow();

	void AttachWindowBody(UGSBWindowBody* InWindowBody);

	UWidget* GetContentWidget() const;
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UNamedSlot* WindowBodySlot;

	UGSBWindowBody* WindowBody;

	UGSBOverlay* GSBOverlay;
};