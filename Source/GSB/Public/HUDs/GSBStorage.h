// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindow.h"
#include "GSBStorage.generated.h"

class UItemStorageComponent;
class UGSBItemList;
class UGSBItemSlot;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStorageAddingItemSlotSignature, UGSBStorage*, Storage, UGSBItemList*, ItemList, UGSBItemSlot*, ItemSlot);

UCLASS()
class GSB_API UGSBStorage : public UGSBWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void OnClosed() override;

public:
	void LinkStorageComponent(UItemStorageComponent* StorageComponent);

	void UnlinkStorageComponent();

	void OnLinkedStorageComponent(UItemStorageComponent* StorageComponent);

	void OnUnlinkedStorageComponent();

	void ClearItemList();

	void AddItemSlot(const struct FItemStack& ItemStack);

	FOnStorageAddingItemSlotSignature OnItemSlotAdded;

protected:
	UItemStorageComponent* LinkedStorageComponent;

private:
	UPROPERTY(meta = (BindWidget))
	UGSBItemList* ItemList;

	UFUNCTION()
	void HandleOnItemSlotAdded(UGSBItemList* InItemList, UGSBItemSlot* ItemSlot);
};
