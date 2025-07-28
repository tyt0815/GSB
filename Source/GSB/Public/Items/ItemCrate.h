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
	virtual void BeginPlay() override;

public:
	void SetItemData(UItemDataAsset* ItemDataAsset);

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "GSB|ItemCrate")
	UItemDataAsset* ItemData;

	UPROPERTY(EditAnywhere, Category = "GSB|ItemCrate")
	class USphereComponent* WidgetShowBounds;


	UPROPERTY(EditAnywhere, Category = "GSB|ItemCrate")
	class UWidgetComponent* HoveredItemNameWidgetComponent;

private	:
	UFUNCTION()
	void OnWidgetShowBoundsBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnWidgetShowBoundsEndOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

	void ShowHoveredItemNameWidget();

	void HideHoveredItemNameWidget();

public:
	FORCEINLINE UItemDataAsset* GetItemData() const
	{
		return ItemData;
	}
};
