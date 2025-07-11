// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBInteractionDescription.generated.h"

class UTextBlock;

UCLASS()
class GSB_API UGSBInteractionDescription : public UUserWidget
{
	GENERATED_BODY()

public:
	void SetDescription(const FText& NewDescription);
	
private:
	UPROPERTY(meta = (BindWidget))
	UTextBlock* Description;

};
