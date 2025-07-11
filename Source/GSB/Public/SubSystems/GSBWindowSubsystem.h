// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSBWindowSubsystem.generated.h"

class UGSBWindowWidget;
class UGSBWindowHead;
class UGSBWindowBody;
class UGSBConfirmationDialog;
class UGSBDialogBody;
class UGSBNumberInputDialogBody;
class UGSBContextMenu;
class AGSBHUD;


UCLASS()
class GSB_API UGSBWindowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGSBWindowWidget* OpenWindow(UObject* InTargetObject, UGSBWindowBody* WindowBody);

	void CloseWindow(UGSBWindowWidget* WindowWidget);

	void CloseAllWindows();

	UGSBConfirmationDialog* OpenConfirmationDialog(UGSBDialogBody* DialogBody, UObject* InTargetObject);

	UGSBNumberInputDialogBody* OpenNumberInputDialog(UObject* InTargetObject, int32 Number);

	bool IsOpened(UGSBWindowWidget* WindowWidget);

	UGSBContextMenu* OpenContextMenu(UObject* InTargetObject);

private	:
	TArray<UGSBWindowWidget*> WindowWidgets;

	AGSBHUD* GetHUD() const;
};
