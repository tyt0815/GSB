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
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_Move;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Move;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_Look;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Look;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_Jump;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Jump;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_Interaction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Interaction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_SelectInteractionScrollUp;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_SelectInteractionScrollDown;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_PreviewFacility;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility1;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility2;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility3;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility4;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility5;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility6;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility7;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility8;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility9;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_PreviewFacility0;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_OnPreviewFacility;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_RotatePreview;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ConfirmFacilityPlacement;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_CancelFacilityPreview;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_ToggleCombatAndBuildMode;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ToggleCombatAndBuildMode;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_ToggleTopDownAndThirdPersonBuildMode;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ToggleTopDownAndThirdPersonBuildMode;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_HandleFacility;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_SelectFacility;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_Combat;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Ability1;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Ability2;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Ability3;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_ToggleInventoryWindow;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ToggleInventoryWindow;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_ToggleConstructibleFacilityListWindow;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_ToggleConstructibleFacilityListWindow;

	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* IMC_SystemUI;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* IA_Esc;
};
