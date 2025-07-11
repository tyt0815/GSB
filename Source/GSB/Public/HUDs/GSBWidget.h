// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "GSBWidget.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBWidget : public UUserWidget
{
	GENERATED_BODY()	
private:
	UObject* TargetObject = nullptr;
public:
	FORCEINLINE UObject* GetTargetObject() const
	{
		return TargetObject;
	}
	FORCEINLINE void SetTargetObject(UObject* InTargetObject)
	{
		TargetObject = InTargetObject;
	}
};
