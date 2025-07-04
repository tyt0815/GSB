// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindowBody.h"
#include "GSBOutputPortDetailWidget.generated.h"

class UGSBItemSlotWidget;
class UGSBStorageWidget;
class UItemStorageComponent;
class UItemDataAsset;
class AConfigurableOutputPort;

UCLASS()
class GSB_API UGSBOutputPortDetailWidget : public UGSBWindowBody
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

public:
	bool TryLinkItemStorageComponent(UItemStorageComponent* ItemStorage);

	void SetSelectedItem(UItemDataAsset* ItemData);

	void UpdateStorageWidget();

	UFUNCTION()
	void HandleStorageItemSlotLeftClicked(UGSBItemSlotWidget* ItemSlotWidget);

	UFUNCTION()
	void ClearSelectedItem(UGSBItemSlotWidget* ItemSlotWidget);

private:
	UPROPERTY(meta = (BindWidget))
	UGSBItemSlotWidget* SelectedItemSlot;

	UPROPERTY(meta = (BindWidget))
	UGSBStorageWidget* Storage;

	AConfigurableOutputPort* LinkedOutputPort;

public:
	FORCEINLINE void SetLinkedOutputPort(AConfigurableOutputPort* NewPort)
	{
		LinkedOutputPort = NewPort;
	}
};
