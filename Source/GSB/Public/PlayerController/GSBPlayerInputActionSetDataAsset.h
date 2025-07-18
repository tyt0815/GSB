// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GSBPlayerInputActionSetDataAsset.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class GSB_API UGSBPlayerInputActionSetDataAsset : public UDataAsset
{
	GENERATED_BODY()
public:
	//////////////////////////////////////////////////////////
	// Default Inputs
	//////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* DefaultGameAndUIInputMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* ToggleInventoryInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* EscInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* DefaultGameOnlyInputMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InteractionInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SelectInteractionScrollUpInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SelectInteractionScrollDownInputAction;

	//////////////////////////////////////////////////////////
	// Build Inputs
	//////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* BuildGameAndUIInputMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SwitchToCombatModeInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* ToggleBuildableFacilityListInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* BuildGameOnlyInputMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* RotatePreview;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* ConfirmFacilityPlacementInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* CancelFacilityPreviewInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* PreviewConveyorBeltInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* PreviewExtensionHubInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* PreviewMiningFacilityInputAction;

	//////////////////////////////////////////////////////////
	// Combat Inputs
	//////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* CombatGameAndUIInputMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SwitchToBuildModeInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* CombatGameOnlyInputMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* Ability1InputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* Ability2InputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* Ability3InputAction;
};
