// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWidget.h"
#include "GSBWindowWidget.generated.h"

class UButton;
class UHorizontalBox;
class UGSBOverlay;
class UGSBWindowBody;
class UGSBWindowHead;

UCLASS()
class GSB_API UGSBWindowWidget : public UGSBWidget
{
	GENERATED_BODY()

public:
	virtual void NativeConstruct() override;

public:
	UFUNCTION()
	void CloseWindow();

	void OnOpenWindow(UGSBOverlay* InGSBOverlay, UObject* InTargetObject);

	void OnCloseWindow();

	void AttachWindowBody(UGSBWindowBody* InWindowBody);

	void ClearWindowHead();

	void AttachWindowHead(UGSBWindowHead* WindowHead);

	UWidget* GetContentWidget() const;
	
private:
	UPROPERTY(meta = (BindWidget))
	UButton* CloseButton;

	UPROPERTY(meta = (BindWidget))
	UHorizontalBox* WindowHeadSlot;

	UPROPERTY(meta = (BindWidget))
	UNamedSlot* WindowBodySlot;

	UGSBWindowBody* WindowBody;

	UGSBOverlay* GSBOverlay;
};