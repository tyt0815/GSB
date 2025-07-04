// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/HUD.h"
#include "GSBHUD.generated.h"

class UGSBOverlay;
class UGSBWindowWidget;
class UGSBWindowBody;
class UGSBConfirmationDialog;

UCLASS()
class GSB_API AGSBHUD : public AHUD
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;

public:
	UGSBWindowWidget* OpenWindow(UGSBWindowBody* InWindowBody);

	void CloseWindow(UGSBWindowWidget* WindowWidget);

	UGSBConfirmationDialog* OpenConfirmationDialog();

	bool IsOpened(UGSBWindowWidget* WindowWidget) const;

	void CloseAllWindows();

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
