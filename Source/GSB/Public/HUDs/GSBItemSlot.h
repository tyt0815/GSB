// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBItemSlot.generated.h"

class UGSBItemSlot;
class UItemDataAsset;
struct FItemStack;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSlotLeftClickedSignature, UGSBItemSlot*, ItemSlotWidget);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FOnItemSlotRightClickedSignature, UGSBItemSlot*, ItemSlotWidget);

UCLASS()
class GSB_API UGSBItemSlot : public UUserWidget
{
	GENERATED_BODY()

public:
	virtual FReply NativeOnPreviewMouseButtonDown(const FGeometry& InGeometry, const FPointerEvent& InMouseEvent) override;

public:
	void SetLabelByItemDataAsset(UItemDataAsset* InItemData);

	void SetLabelByItemStack(const FItemStack& ItemStack);

	FOnItemSlotLeftClickedSignature OnItemSlotLeftClicked;

	FOnItemSlotRightClickedSignature OnItemSlotRightClicked;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* Label;

	UItemDataAsset* ItemData;

public:
	FORCEINLINE UItemDataAsset* GetItemData() const
	{
		return ItemData;
	}
};
