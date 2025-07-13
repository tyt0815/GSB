// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HUDs/GSBItemSlot.h"
#include "GSBItemList.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_TwoParams(FOnItemListAddingItemSlotSignature, UGSBItemList*, ItemList, UGSBItemSlot*, ItemSlot);

UCLASS()
class GSB_API UGSBItemList : public UUserWidget
{
	GENERATED_BODY()

public:
	void ClearItemList();

	void AddItemSlot(const FItemStack& ItemStack);

	FOnItemListAddingItemSlotSignature OnItemSlotAdded;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UScrollBox* ItemList;

	UPROPERTY(EditDefaultsOnly, Category = "SubClasses")
	TSubclassOf<UGSBItemSlot> ItemSlotClass;
};
