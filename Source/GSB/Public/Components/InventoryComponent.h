// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ItemStorageComponent.h"
#include "InventoryComponent.generated.h"


//
// 캐릭터 Inventory용으로 사용할 컴포넌트. 기존 UItemStorageComponent에 중량 제한을 추가한 버전이다.
//
UCLASS()
class GSB_API UInventoryComponent : public UItemStorageComponent
{
	GENERATED_BODY()
	
public:
	virtual int32 GetAddableStackCount(const FItemStack& ItemStack) const override;

	virtual int32 StoreItem(const FItemStack& Item) override;

	virtual int32 UnstoreItem(const FItemStack& Item) override;

	virtual void UpdateStorageWidget() override;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UInventoryComponent")
	int32 MaxWeight = 1000;

	UPROPERTY(VisibleAnywhere, Category = "UInventoryComponent")
	int32 CurrentWeight = 0;

private:
	void UpdateInventoryWidget();

public:
	FORCEINLINE int32 GetMaxWeight() const
	{
		return MaxWeight;
	}

	FORCEINLINE int32 GetCurrentWeight() const
	{
		return CurrentWeight;
	}
};
