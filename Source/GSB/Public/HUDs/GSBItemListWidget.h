// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWidget.h"
#include "HUDs/GSBItemSlotWidget.h"
#include "Items/ItemDataAsset.h"
#include "GSBItemListWidget.generated.h"

class UScrollBox;
class UGSBItemSlotWidget;
class UGSBStorageWidget;

UCLASS()
class GSB_API UGSBItemListWidget : public UGSBWidget
{
	GENERATED_BODY()

public:
	void ClearItemList();

	FOnItemSlotClickedDelegates* AddItemSlot(const FItemStack& ItemStack);

	void OnAttachedToStorageWidget(UGSBStorageWidget* ParentStorageWidget);
	
private:
	UPROPERTY(meta = (BindWidget))
	UScrollBox* ItemList;

	UPROPERTY(EditDefaultsOnly, Category = "UGSBItemListWidget")
	TSubclassOf<UGSBItemSlotWidget> ItemSlotClass;
};
