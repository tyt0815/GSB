// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSBWindowSubsystem.generated.h"

class UGSBWindowWidget;
class UGSBWindowBody;
class UGSBConfirmationDialog;
class AGSBHUD;


UCLASS()
class GSB_API UGSBWindowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGSBWindowWidget* OpenWindow(UGSBWindowBody* WindowBody);

	void CloseWindow(UGSBWindowWidget* WindowWidget);

	UGSBConfirmationDialog* OpenConfirmationDialog();

	bool IsOpened(UGSBWindowWidget* WindowWidget);

private	:
	TArray<UGSBWindowWidget*> WindowWidgets;

	AGSBHUD* GetHUD() const;
};
