// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindow.h"
#include "GSBOutputPortDetailWindow.generated.h"

class AConfigurableOutputPort;
class UGSBStorageBody;
class UGSBItemList;
class UGSBItemSlot;

UCLASS()
class GSB_API UGSBOutputPortDetailWindow : public UGSBWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;


public:
	void LinkOutputPort(AConfigurableOutputPort* Port);

	void SetOutputItem(class UItemDataAsset* ItemData);

private:
	UFUNCTION()
	void HandleOnItemSlotAdded(UGSBStorageBody* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnItemSlotLeftClicked(UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnOutputItemSlotClicked(UGSBItemSlot* ItemSlot);

	UPROPERTY(meta = (BindWidget))
	UGSBStorageBody* HubStorage;

	UPROPERTY(meta = (BindWidget))
	UGSBItemSlot* OutputItemSlot;

	AConfigurableOutputPort* LinkedOutputPort;
};
