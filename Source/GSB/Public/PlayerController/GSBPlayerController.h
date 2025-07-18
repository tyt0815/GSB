// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GSBPlayerController.generated.h"

class UWidget;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class UGSBPlayerInputActionSetDataAsset;

enum class EGamePlayMode : uint8
{
	EGPM_CombatGameOnly,
	EGPM_CombatGameAndUI,
	EGPM_BuildGameOnly,
	EGPM_BuildGameAndUI
};

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

	bool IsUIControlMode() const;

	bool IsCombatMode() const;

	bool IsBuildMode() const;

	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();

protected:
	UPROPERTY(EditAnywhere)
	UGSBPlayerInputActionSetDataAsset* PlayerInputSet;

	

private:
	void AddDefaultGameOnlyInputContexts();

	void AddDefaultGameAndUIInputContexts();

	void AddCombatGameOnlyInputContexts();

	void AddCombatGameAndUIInputContexts();;

	void AddBuildGameOnlyInputContexts();

	void AddBuildGameAndUIInputContexts();

	void EnterUIControlMode();

	void ExitUIControlMode();

	EGamePlayMode GamePlayMode;

	TArray<UInputMappingContext*> CurrentInputMappingContexts;
	TArray<UInputMappingContext*> StoredInputMappingContexts;

public:
	FORCEINLINE UGSBPlayerInputActionSetDataAsset* GetPlayerInputSet() const
	{
		return PlayerInputSet;
	}
};
