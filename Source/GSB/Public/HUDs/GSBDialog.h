// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HUDs/GSBWindowBody.h"
#include "GSBDialog.generated.h"

class UGSBDialogBody;

UCLASS()
class GSB_API UGSBDialog : public UGSBWindowBody
{
	GENERATED_BODY()

public:
	virtual void NativeOnFocusChanging(const FWeakWidgetPath& PreviousFocusPath, const FWidgetPath& NewWidgetPath, const FFocusEvent& InFocusEvent) override;

public:
	void AttachDialogBody(UGSBDialogBody* DialogBody);

	UGSBDialogBody* GetDialogBody() const;
	
protected:
	UPROPERTY(meta = (BindWidget))
	UNamedSlot* DialogBodySlot;

	UGSBDialogBody* AttachedDialogBody;

public:
	template<typename T>
	FORCEINLINE T* GetDialogBody() const
	{
		return Cast<T>(GetDialogBody());
	}
};
