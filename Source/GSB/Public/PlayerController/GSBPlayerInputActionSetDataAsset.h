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
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* ToggleCombatAndBuildModeInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* InteractionInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SelectInteractionScrollUpInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SelectInteractionScrollDownInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* ToggleInventoryInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* EscInputAction;



	//////////////////////////////////////////////////////////
	// Build Inputs
	//////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* BuildInputMapping;

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
	UInputMappingContext* CombatInputMapping;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* Ability1InputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* Ability2InputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* Ability3InputAction;
};
