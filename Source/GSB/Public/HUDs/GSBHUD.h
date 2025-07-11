// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GSBHUD.generated.h"

class UGSBOverlay;
class UGSBWindowWidget;
class UGSBWindowBody;
class UGSBConfirmationDialog;
class UGSBDialogBody;
class UGSBNumberInputDialogBody;
class UGSBContextMenu;

DECLARE_DYNAMIC_MULTICAST_DELEGATE(FEndBeginPlaySignature);

UCLASS()
class GSB_API AGSBHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	UGSBWindowWidget* OpenWindow_Internal(UObject* InTargetObject);

	void CloseWindow_Internal(UGSBWindowWidget* WindowWidget);

	UGSBConfirmationDialog* OpenConfirmationDialog_Internal(UGSBDialogBody* Body, UObject* InTargetObject);

	UGSBNumberInputDialogBody* OpenNumberInputDialog_Internal(UObject* InTargetObject, int32 Number);

	bool IsOpened_Internal(UGSBWindowWidget* WindowWidget) const;

	void CloseAllWindows_Internal();

	UGSBContextMenu* OpenContextMenu_Internal(UObject* InTargetObject);

	FEndBeginPlaySignature OnEndBeginPlay;

protected:
	UPROPERTY(EditDefaultsOnly, Category = "AGSBHUD")
	TSubclassOf<UGSBOverlay> OverlayWidgetClass;

	UPROPERTY()
	UGSBOverlay* OverlayWidget;

public:
	FORCEINLINE UGSBOverlay* GetOverlayWidget() const
	{
		return OverlayWidget;
	}	
};
