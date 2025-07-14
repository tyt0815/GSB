// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GSBPlayerController.generated.h"

class UWidget;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class UGSBPlayerInputActionSetDataAsset;

UCLASS()
class GSB_API AGSBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
public:
	void ActivateCombatInputContext();

	void ActivateBuildInputContext();

	void AddInputMappingContext(UInputMappingContext* InputMappingContext);

	void AddInputMappingContexts(const TArray<UInputMappingContext*> Contexts);

	void RemoveInputMappingContext(UInputMappingContext* InputMappingContext);

	void ClearAllInputMappingContext();

	void StoreInputMappingContexts(const TArray<UInputMappingContext*> Contexts);

	void SetUIControlMode(bool bUI);

	void FocusToWidget(UWidget* Widget);

protected:
	UPROPERTY(EditAnywhere)
	UGSBPlayerInputActionSetDataAsset* PlayerInputSet;

	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem;

private:
	void EnterUIControlMode();

	void ExitUIControlMode();

	bool bUIControlMode = false;

	TArray<UInputMappingContext*> CurrentInputMappingContexts;
	TArray<UInputMappingContext*> StoredInputMappingContexts;

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
