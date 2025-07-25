// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GSBPlayerController.generated.h"

class UWidget;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class UGSBPlayerInputActionSetDataAsset;
class UGSBTopDownBuildPawnInputSet;

enum class EGamePlayMode : uint8
{
	EGPM_CombatGameOnly,
	EGPM_CombatGameAndUI,
	EGPM_CharacterBuildGameOnly,
	EGPM_CharacterGameAndUI,
	EGPM_TopDownBuild
};

UCLASS()
class GSB_API AGSBPlayerController : public APlayerController
{
	GENERATED_BODY()

public:
	virtual void BeginPlay() override;
	
public:
	void ActivateCombatInputContext();

	void ActivateCharacterBuildInputContext();

	void ActivateTopDownBuildInputContext();

	void AddInputMappingContext(UInputMappingContext* InputMappingContext);

	void AddInputMappingContexts(const TArray<UInputMappingContext*> Contexts);

	void RemoveInputMappingContext(UInputMappingContext* InputMappingContext);

	void ClearAllInputMappingContext();

	void StoreInputMappingContexts(const TArray<UInputMappingContext*> Contexts);

	void SetUIControlMode(bool bUI);

	bool IsUIControlMode() const;

	bool IsCombatMode() const;

	bool IsCharacterBuildMode() const;

	bool IsTopDownBuildMode() const;

	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();

protected:
	UPROPERTY(EditAnywhere)
	UGSBPlayerInputActionSetDataAsset* PlayerInputSet;

	UPROPERTY(EditAnywhere)
	UGSBTopDownBuildPawnInputSet* TopDownBuildPawnInputSet;

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
	FORCEINLINE UGSBTopDownBuildPawnInputSet* GetTopDownBuildPawnInputSet() const
	{
		return TopDownBuildPawnInputSet;
	}
};
