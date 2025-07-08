// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GSBPlayerController.generated.h"

class UWidget;
class UInputMappingContext;
class UGSBPlayerInputActionSetDataAsset;

UCLASS()
class GSB_API AGSBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void ActivateCombatInputContext();

	void ActivateBuildInputContext();

	void AddInputMappingContext(UInputMappingContext* InputMappingContext);

	void RemoveInputMappingContext(UInputMappingContext* InputMappingContext);

	void ClearAllInputMappingContext();

	void SetUIControlMode(bool bUI);

	void FocusToWidget(UWidget* Widget);

protected:
	UPROPERTY(EditAnywhere)
	UGSBPlayerInputActionSetDataAsset* PlayerInputSet;

private:
	void EnterUIControlMode();

	void ExitUIControlMode();

	bool bUIControlMode = false;

public:
	FORCEINLINE bool IsUIControlMode() const
	{
		return bUIControlMode;
	}
	FORCEINLINE UGSBPlayerInputActionSetDataAsset* GetPlayerInputSet() const
	{
		return PlayerInputSet;
	}
};
