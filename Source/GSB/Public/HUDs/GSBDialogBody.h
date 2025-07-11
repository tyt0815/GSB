// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWidget.h"
#include "GSBDialogBody.generated.h"

class UGSBDialog;

UCLASS()
class GSB_API UGSBDialogBody : public UGSBWidget
{
	GENERATED_BODY()
	
public:
	virtual void OnAttachedToDialog(UGSBDialog* Dialog);

private:
	UGSBDialog* ParentDialog;

public:
	template<typename T = UGSBDialog>
	FORCEINLINE T* GetParentDialog() const
	{
		return Cast<T>(ParentDialog);
	}
};
