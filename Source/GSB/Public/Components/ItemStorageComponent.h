// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Items/ItemDataAsset.h"
#include "ItemStorageComponent.generated.h"

class UGSBStorageBody;

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSB_API UItemStorageComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemStorageComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	virtual void BeginPlay() override;

public:
	virtual int32 GetAddableStackCount(const FItemStack& ItemStack) const;

	FItemStack GetItemStack(UItemDataAsset* ItemData) const;

	int32 FindStoredItemIndex(UItemDataAsset* ItemData) const;

	// @return 저장한 아이템 수
	virtual int32 StoreItem(const FItemStack& Item);

	// @return 제거한 아이템 수
	virtual int32 UnstoreItem(const FItemStack& Item);

	int32 DropItem(FItemStack ItemStack);

	// @return 옮긴 아이템수
	int32 MoveItemTo(UItemStorageComponent* To, FItemStack ItemStack);

	int32 MoveAllItemTo(UItemStorageComponent* To, UItemDataAsset* ItemData);

protected:
	UPROPERTY(VisibleAnywhere, Category = "ItemStorage")
	TArray<FItemStack> StoredItems;

	UPROPERTY(EditAnywhere, Category = "ItemStorage")
	int32 StorageSize = 50;

	/////////////////////////////////////////////////////////////////////////
	// UI
	/////////////////////////////////////////////////////////////////////////
public:
	void LinkStorageWidget(UGSBStorageBody* StorageWidget);

	void UnlinkStorageWidget();

	virtual void UpdateStorageWidget();

	UGSBStorageBody* LinkedStorageWidget;

protected:
	
public:
	FORCEINLINE FItemStack GetItemStackAt(int32 Index) const
	{
		return StoredItems[Index];
	}
	FORCEINLINE int32 GetStoredItemTypeCount() const
	{
		return StoredItems.Num();
	}
	FORCEINLINE bool IsEmpty() const
	{
		return StoredItems.IsEmpty();
	}
	FORCEINLINE void SetStorageSize(int32 NewSize)
	{
		StorageSize = NewSize;
	}
	FORCEINLINE const TArray<FItemStack>& GetStoredItems() const
	{
		return StoredItems;
	}
};
