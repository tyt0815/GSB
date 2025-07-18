// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/GSBPlayerController.h"
#include "PlayerController/GSBPlayerInputActionSetDataAsset.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "Components/Widget.h"
#include "DebugHeader.h"

void AGSBPlayerController::BeginPlay()
{
	Super::BeginPlay();
}

void AGSBPlayerController::ActivateCombatInputContext()
{
	ClearAllInputMappingContext();
	if (IsUIControlMode())
	{
		GamePlayMode = EGamePlayMode::EGPM_CombatGameAndUI;

		AddCombatGameAndUIInputContexts();
	}
	else
	{
		GamePlayMode = EGamePlayMode::EGPM_CombatGameOnly;

		AddCombatGameOnlyInputContexts();
	}
}

void AGSBPlayerController::ActivateBuildInputContext()
{
	ClearAllInputMappingContext();
	if (IsUIControlMode())
	{
		GamePlayMode = EGamePlayMode::EGPM_BuildGameAndUI;

		AddBuildGameAndUIInputContexts();
	}
	else
	{
		GamePlayMode = EGamePlayMode::EGPM_BuildGameOnly;

		AddBuildGameOnlyInputContexts();
	}
}

void AGSBPlayerController::AddInputMappingContext(UInputMappingContext* InputMappingContext)
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = GetEnhancedInputLocalPlayerSubsystem())
	{
		if (!EnhancedInputLocalPlayerSubsystem->HasMappingContext(InputMappingContext))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
			CurrentInputMappingContexts.Add(InputMappingContext);
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
			CurrentInputMappingContexts.Remove(InputMappingContext);
		}
	}
}

void AGSBPlayerController::ClearAllInputMappingContext()
{
	for (UInputMappingContext* Context : CurrentInputMappingContexts)
	{
		RemoveInputMappingContext(Context);
	}
}

void AGSBPlayerController::StoreInputMappingContexts(const TArray<UInputMappingContext*> Contexts)
{
	StoredInputMappingContexts.Empty();
	for (UInputMappingContext* Context : Contexts)
	{
		StoredInputMappingContexts.Add(Context);
	}
}

void AGSBPlayerController::SetUIControlMode(bool bUI)
{
	if (bUI)
	{
		EnterUIControlMode();
	}
	else
	{
		ExitUIControlMode();
	}
}

bool AGSBPlayerController::IsUIControlMode() const
{
	return GamePlayMode == EGamePlayMode::EGPM_BuildGameAndUI ||
		GamePlayMode == EGamePlayMode::EGPM_CombatGameAndUI;
}

bool AGSBPlayerController::IsCombatMode() const
{
	return GamePlayMode == EGamePlayMode::EGPM_CombatGameAndUI ||
		GamePlayMode == EGamePlayMode::EGPM_CombatGameOnly;
}

bool AGSBPlayerController::IsBuildMode() const
{
	return GamePlayMode == EGamePlayMode::EGPM_BuildGameAndUI ||
		GamePlayMode == EGamePlayMode::EGPM_BuildGameOnly;
}

UEnhancedInputLocalPlayerSubsystem* AGSBPlayerController::GetEnhancedInputLocalPlayerSubsystem()
{
	return ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
}

void AGSBPlayerController::AddDefaultGameOnlyInputContexts()
{
	AddDefaultGameAndUIInputContexts();
	AddInputMappingContext(PlayerInputSet->DefaultGameOnlyInputMapping);
}

void AGSBPlayerController::AddDefaultGameAndUIInputContexts()
{
	AddInputMappingContext(PlayerInputSet->DefaultGameAndUIInputMapping);
}

void AGSBPlayerController::AddCombatGameOnlyInputContexts()
{
	AddDefaultGameOnlyInputContexts();
	AddCombatGameAndUIInputContexts();
	AddInputMappingContext(PlayerInputSet->CombatGameOnlyInputMapping);
}

void AGSBPlayerController::AddCombatGameAndUIInputContexts()
{
	AddDefaultGameAndUIInputContexts();
	AddInputMappingContext(PlayerInputSet->CombatGameAndUIInputMapping);
}

void AGSBPlayerController::AddBuildGameOnlyInputContexts()
{
	AddDefaultGameOnlyInputContexts();
	AddBuildGameAndUIInputContexts();
	AddInputMappingContext(PlayerInputSet->BuildGameOnlyInputMapping);
}

void AGSBPlayerController::AddBuildGameAndUIInputContexts()
{
	AddDefaultGameAndUIInputContexts();
	AddInputMappingContext(PlayerInputSet->BuildGameAndUIInputMapping);
}

void AGSBPlayerController::EnterUIControlMode()
{
	SetShowMouseCursor(true);

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	InputMode.SetHideCursorDuringCapture(false);
	SetInputMode(InputMode);

	if (IsBuildMode())
	{
		GamePlayMode = EGamePlayMode::EGPM_BuildGameAndUI;

		ClearAllInputMappingContext();
		AddBuildGameAndUIInputContexts();
	}
	else
	{
		GamePlayMode = EGamePlayMode::EGPM_CombatGameAndUI;

		ClearAllInputMappingContext();
		AddCombatGameAndUIInputContexts();
	}
}

void AGSBPlayerController::ExitUIControlMode()
{
	SetShowMouseCursor(false);

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	ClearAllInputMappingContext();
	AddInputMappingContexts(StoredInputMappingContexts);

	if (IsBuildMode())
	{
		GamePlayMode = EGamePlayMode::EGPM_BuildGameOnly;

		ClearAllInputMappingContext();
		AddBuildGameOnlyInputContexts();
	}
	else
	{
		GamePlayMode = EGamePlayMode::EGPM_CombatGameOnly;

		ClearAllInputMappingContext();
		AddCombatGameOnlyInputContexts();
	}
}
