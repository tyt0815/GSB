// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindow.h"
#include "GSBStorage.generated.h"

class UGSBStorage;
class UGSBStorageBody;
class UItemStorageComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnStorageAddItemSlotSignature, UGSBStorage*, Storage, UGSBStorageBody*, StorageBody, UGSBItemList*, ItemList, UGSBItemSlot*, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStorageOpenItemSlotContextMenuSignature, UGSBStorage*, Storage, UGSBStorageBody*, StorageBody, UGSBContextMenu*, ContextMenu);

UCLASS()
class GSB_API UGSBStorage : public UGSBWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	virtual void OnClosed() override;

	void LinkStorageComponent(UItemStorageComponent* StorageComponent);

	void AddItemSlotContextMenuEntry_DropItem();

public:
	FOnStorageAddItemSlotSignature OnItemSlotAdded;

	FOnStorageOpenItemSlotContextMenuSignature OnItemSlotContextMenuOpened;

private:
	UPROPERTY(meta = (BindWidget))
	UGSBStorageBody* StorageBody;

	UFUNCTION()
	void HandleOnItemSlotAdded(UGSBStorageBody* InStorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnItemSlotContextMenuOpened(UGSBStorageBody* InStorageBody, UGSBContextMenu* ContextMenu);
};
