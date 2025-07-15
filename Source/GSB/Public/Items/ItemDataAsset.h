// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "ItemDataAsset.generated.h"

UCLASS()
class GSB_API UItemDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	FText Name;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	int32 MaxStackCount = 999999;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	int32 Weight = 1;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "ItemData")
	TSubclassOf<AActor> ItemClass;



};

USTRUCT(BlueprintType)
struct FItemStack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	UItemDataAsset* ItemData;
	
	UPROPERTY(EditAnywhere)
	int32 Stack;
};

FORCEINLINE int32 CalcItemStackWeight(const FItemStack& ItemStack)
{
	return IsValid(ItemStack.ItemData) ? ItemStack.Stack * ItemStack.ItemData->Weight : 0;
}