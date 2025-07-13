// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GSBWindowSubsystem.generated.h"

class UGSBWindow;
class AGSBHUD;


UCLASS()
class GSB_API UGSBWindowSubsystem : public UGameInstanceSubsystem
{
	GENERATED_BODY()

public:
	UGSBWindow* OpenWindow(UClass* WindowClass, const FName& WindowName);

	void CloseWindow(UGSBWindow* Window);

	bool IsWindowOpened(UGSBWindow* Window);

	void CloseAllWindows();

private	:
	AGSBHUD* HUD;

	AGSBHUD* GetHUD();
};
