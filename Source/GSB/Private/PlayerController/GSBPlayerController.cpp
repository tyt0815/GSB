// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/GSBPlayerController.h"
#include "PlayerController/GSBPlayerInputActionSetDataAsset.h"
#include "PlayerController/GSBTopDownBuildPawnInputSet.h"
#include "BuildSystem/TopDownBuildPawn.h"
#include "Characters/GSBPlayer.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/Widget.h"
#include "DebugHeader.h"

void AGSBPlayerController::BeginPlay()
{
	Super::BeginPlay();

	SetGamePlayMode_PlayerCombatGameOnly();
}

UEnhancedInputLocalPlayerSubsystem* AGSBPlayerController::GetEnhancedInputLocalPlayerSubsystem()
{
	return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
}

void AGSBPlayerController::AddInputMappingContext(UInputMappingContext* InputMappingContext)
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = GetEnhancedInputLocalPlayerSubsystem())
	{
		if (!EnhancedInputLocalPlayerSubsystem->HasMappingContext(InputMappingContext))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AGSBPlayerController::AddInputMappingContexts(const TArray<UInputMappingContext*> Contexts)
{
	for (UInputMappingContext* Context : Contexts)
	{
		AddInputMappingContext(Context);
	}
}

void AGSBPlayerController::RemoveInputMappingContext(UInputMappingContext* InputMappingContext)
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = GetEnhancedInputLocalPlayerSubsystem())
	{
		if (EnhancedInputLocalPlayerSubsystem->HasMappingContext(InputMappingContext))
		{
			EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

void AGSBPlayerController::ClearAllInputMappingContext()
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = GetEnhancedInputLocalPlayerSubsystem())
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
	}
}

void AGSBPlayerController::SwitchGamePlayMode_PlayerCombatGameOnly(AGSBPlayer* GSBPlayer)
{
	SetGamePlayMode_PlayerCombatGameOnly();

	if (IsValid(GSBPlayer))
	{
		if (GetPawn() != GSBPlayer)
		{
			Possess(GSBPlayer);
		}
		GSBPlayer->OnEnterCombatModeGameOnly();
	}
}

void AGSBPlayerController::SwitchGamePlayMode_PlayerCombatGameAndUI(AGSBPlayer* GSBPlayer)
{
	SetGamePlayMode_PlayerCombatGameAndUI();

	if (IsValid(GSBPlayer))
	{
		if (GetPawn() != GSBPlayer)
		{
			Possess(GSBPlayer);
		}
		GSBPlayer->OnEnterCombatModeGameAndUI();
	}
}

void AGSBPlayerController::SwitchGamePlayMode_PlayerBuildGameOnly(AGSBPlayer* GSBPlayer)
{
	SetGamePlayMode_PlayerBuildGameOnly();

	if (IsValid(GSBPlayer))
	{
		if (GetPawn() != GSBPlayer)
		{
			Possess(GSBPlayer);
		}
		GSBPlayer->OnEnterBuildModeGameOnly();
	}
}

void AGSBPlayerController::SwitchGamePlayMode_PlayerBuildGameAndUI(AGSBPlayer* GSBPlayer)
{
	SetGamePlayMode_PlayerBuildGameAndUI();

	if (IsValid(GSBPlayer))
	{
		if (GetPawn() != GSBPlayer)
		{
			Possess(GSBPlayer);
		}
		GSBPlayer->OnEnterBuildModeGameAndUI();
	}
}

void AGSBPlayerController::SwitchGamePlayMode_TopDownBuildGameAndUI(ATopDownBuildPawn* TopDownBuildPawn)
{
	SetGamePlayMode_TopDownBuildGameAndUI();

	if (IsValid(TopDownBuildPawn))
	{
		if (GetPawn() != TopDownBuildPawn)
		{
			Possess(TopDownBuildPawn);
		}
		TopDownBuildPawn->OnEnterTopDownBuildModeGameAndUI();
	}
}

void AGSBPlayerController::SetGamePlayMode_PlayerCombatGameOnly()
{
	ClearAllInputMappingContext();

	AddInputMappingContext(InputSet->IMC_Move);
	AddInputMappingContext(InputSet->IMC_Look);
	AddInputMappingContext(InputSet->IMC_Jump);
	AddInputMappingContext(InputSet->IMC_Interaction);
	AddInputMappingContext(InputSet->IMC_ToggleCombatAndBuildMode);
	AddInputMappingContext(InputSet->IMC_Combat);
	AddInputMappingContext(InputSet->IMC_ToggleInventoryWindow);
	AddInputMappingContext(InputSet->IMC_SystemUI);

	ControlledPawnType = EControlledPawn::ECP_GSBPlayer;
	GSBPlayerGameMode = EGSBPlayerMode::EPGM_Combat;
	SetInputMode_GameOnly();
}

void AGSBPlayerController::SetGamePlayMode_PlayerCombatGameAndUI()
{
	ClearAllInputMappingContext();

	AddInputMappingContext(InputSet->IMC_Move);
	AddInputMappingContext(InputSet->IMC_Jump);
	AddInputMappingContext(InputSet->IMC_ToggleInventoryWindow);
	AddInputMappingContext(InputSet->IMC_SystemUI);

	ControlledPawnType = EControlledPawn::ECP_GSBPlayer;
	GSBPlayerGameMode = EGSBPlayerMode::EPGM_Combat;
	SetInputMode_GameAndUI();
}

void AGSBPlayerController::SetGamePlayMode_PlayerBuildGameOnly()
{
	ClearAllInputMappingContext();

	AddInputMappingContext(InputSet->IMC_Move);
	AddInputMappingContext(InputSet->IMC_Look);
	AddInputMappingContext(InputSet->IMC_Jump);
	AddInputMappingContext(InputSet->IMC_Interaction);
	AddInputMappingContext(InputSet->IMC_ToggleCombatAndBuildMode);
	AddInputMappingContext(InputSet->IMC_ToggleTopDownAndThirdPersonBuildMode);
	AddInputMappingContext(InputSet->IMC_BuildFacility);
	AddInputMappingContext(InputSet->IMC_ToggleInventoryWindow);
	AddInputMappingContext(InputSet->IMC_ToggleConstructibleFacilityListWindow);
	AddInputMappingContext(InputSet->IMC_SystemUI);

	ControlledPawnType = EControlledPawn::ECP_GSBPlayer;
	GSBPlayerGameMode = EGSBPlayerMode::EPGM_Build;
	SetInputMode_GameOnly();
}

void AGSBPlayerController::SetGamePlayMode_PlayerBuildGameAndUI()
{
	ClearAllInputMappingContext();

	AddInputMappingContext(InputSet->IMC_Move);
	AddInputMappingContext(InputSet->IMC_Jump);
	AddInputMappingContext(InputSet->IMC_ToggleInventoryWindow);
	AddInputMappingContext(InputSet->IMC_ToggleConstructibleFacilityListWindow);
	AddInputMappingContext(InputSet->IMC_SystemUI);

	ControlledPawnType = EControlledPawn::ECP_GSBPlayer;
	GSBPlayerGameMode = EGSBPlayerMode::EPGM_Build;
	SetInputMode_GameAndUI();
}

void AGSBPlayerController::SetGamePlayMode_TopDownBuildGameAndUI()
{
	ClearAllInputMappingContext();

	AddInputMappingContext(InputSet->IMC_Move);
	AddInputMappingContext(InputSet->IMC_BuildFacility);
	AddInputMappingContext(InputSet->IMC_ToggleConstructibleFacilityListWindow);
	AddInputMappingContext(InputSet->IMC_ToggleTopDownAndThirdPersonBuildMode);
	AddInputMappingContext(InputSet->IMC_SystemUI);

	ControlledPawnType = EControlledPawn::ECP_TopDownBuildPawn;
	SetInputMode_GameAndUI();
}

bool AGSBPlayerController::IsGSBPlayerControlled() const
{
	return ControlledPawnType == EControlledPawn::ECP_GSBPlayer;
}

bool AGSBPlayerController::IsTopDownBuildPawnControlled() const
{
	return ControlledPawnType == EControlledPawn::ECP_TopDownBuildPawn;
}

bool AGSBPlayerController::IsPlayerCombatMode() const
{
	return IsGSBPlayerControlled() && GSBPlayerGameMode == EGSBPlayerMode::EPGM_Combat;
}

bool AGSBPlayerController::IsPlayerBuildMode() const
{
	return IsGSBPlayerControlled() && GSBPlayerGameMode == EGSBPlayerMode::EPGM_Build;
}

void AGSBPlayerController::SetInputMode_GameOnly()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	InputModeType = EInputModeType::EIMT_GameOnly;
}

void AGSBPlayerController::SetInputMode_UIOnly()
{
	FInputModeUIOnly InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	SetInputMode(InputMode);

	InputModeType = EInputModeType::EIMT_UIOnly;
}

void AGSBPlayerController::SetInputMode_GameAndUI()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	InputModeType = EInputModeType::EIMT_GameAndUI;
}