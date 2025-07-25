// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemCrate.generated.h"

class UItemDataAsset;

UCLASS()
class GSB_API AItemCrate : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemCrate();

	class ADroppedItem* ConvertToDroppedItem();

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "AItemCrate")
	UItemDataAsset* ItemData;

public:
	FORCEINLINE UItemDataAsset* GetItemData() const
	{
		return ItemData;
	}
	FORCEINLINE void SetItemData(UItemDataAsset* ItemDataAsset)
	{
		ItemData = ItemDataAsset;
	}

};
