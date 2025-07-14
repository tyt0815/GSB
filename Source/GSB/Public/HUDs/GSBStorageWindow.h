// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindow.h"
#include "GSBStorageWindow.generated.h"

class UGSBStorageWindow;
class UGSBStorage;
class UItemStorageComponent;

DECLARE_DYNAMIC_MULTICAST_DELEGATE_FourParams(FOnStorageAddItemSlotSignature, UGSBStorageWindow*, Storage, UGSBStorage*, StorageBody, UGSBItemList*, ItemList, UGSBItemSlot*, ItemSlot);
DECLARE_DYNAMIC_MULTICAST_DELEGATE_ThreeParams(FOnStorageOpenItemSlotContextMenuSignature, UGSBStorageWindow*, Storage, UGSBStorage*, StorageBody, UGSBContextMenu*, ContextMenu);

UCLASS()
class GSB_API UGSBStorageWindow : public UGSBWindow
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;

	void LinkStorageComponent(UItemStorageComponent* StorageComponent);

	void AddItemSlotContextMenuEntry_DropItem();

public:
	FOnStorageAddItemSlotSignature OnItemSlotAdded;

	FOnStorageOpenItemSlotContextMenuSignature OnItemSlotContextMenuOpened;

private:
	UPROPERTY(meta = (BindWidget))
	UGSBStorage* StorageBody;

	UFUNCTION()
	void HandleOnItemSlotAdded(UGSBStorage* InStorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	UFUNCTION()
	void HandleOnItemSlotContextMenuOpened(UGSBStorage* InStorageBody, UGSBContextMenu* ContextMenu);
};
