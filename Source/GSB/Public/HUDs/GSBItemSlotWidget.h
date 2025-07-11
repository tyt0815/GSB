// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWidget.h"
#include "Items/ItemDataAsset.h"
#include "GSBItemSlotWidget.generated.h"

class UButton;
class UTextBlock;
class UGSBItemSlotWidget;
class UGSBItemListWidget;
class UGSBStorageWidget;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSlotLeftClicked, UGSBItemSlotWidget*, ItemSlotWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSlotRightClicked, UGSBItemSlotWidget*, ItemSlotWidget);

struct FOnItemSlotClickedDelegates
{
	FOnItemSlotLeftClicked OnItemSlotLeftClicked;
	FOnItemSlotRightClicked OnItemSlotRightClicked;
};

UCLASS()
class GSB_API UGSBItemSlotWidget : public UGSBWidget
{
	GENERATED_BODY()
public:
	void UpdateItemData(const FItemStack& ItemStack);

	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;
	
	FOnItemSlotClickedDelegates OnItemSlotClickedDelegates;

	UGSBItemListWidget* GetLinkedItemListWidget() const;

	void OnAttachedToItemList(UGSBItemListWidget* ParentItemList);

private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* ItemNameAndStack;

	TWeakObjectPtr<UItemDataAsset> ItemData;

public:
	FORCEINLINE UItemDataAsset* GetItemData() const
	{
		return ItemData.Get();
	}
};
