// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "HoveredItemName.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UHoveredItemName : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetItemName(const FText& NewItemName);
	

private	:
	UPROPERTY(meta = (BindWidget))
	class UTextBlock* ItemName;
};
