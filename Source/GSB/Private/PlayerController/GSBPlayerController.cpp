// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/GSBPlayerController.h"
#include "PlayerController/GSBPlayerInputActionSetDataAsset.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Widget.h"

void AGSBPlayerController::ActivateCombatInputContext()
{
	ClearAllInputMappingContext();
	AddInputMappingContext(PlayerInputSet->DefaultInputMapping);
	AddInputMappingContext(PlayerInputSet->CombatInputMapping);
}

void AGSBPlayerController::ActivateBuildInputContext()
{
	ClearAllInputMappingContext();
	AddInputMappingContext(PlayerInputSet->DefaultInputMapping);
	AddInputMappingContext(PlayerInputSet->BuildInputMapping);
}

void AGSBPlayerController::AddInputMappingContext(UInputMappingContext* InputMappingContext)
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (!EnhancedInputLocalPlayerSubsystem->HasMappingContext(InputMappingContext))
		{
			EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		}
	}
}

void AGSBPlayerController::RemoveInputMappingContext(UInputMappingContext* InputMappingContext)
{
	if (UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer()))
	{
		if (EnhancedInputLocalPlayerSubsystem->HasMappingContext(InputMappingContext))
		{
			EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
		}
	}
}

void AGSBPlayerController::ClearAllInputMappingContext()
{
	UEnhancedInputLocalPlayerSubsystem* EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (EnhancedInputLocalPlayerSubsystem)
	{
		EnhancedInputLocalPlayerSubsystem->ClearAllMappings();
	}
}

void AGSBPlayerController::SetUIControlMode(bool bUI)
{
	bUIControlMode = bUI;
	if (bUI)
	{
		EnterUIControlMode();
	}
	else
	{
		ExitUIControlMode();
	}
}

void AGSBPlayerController::FocusToWidget(UWidget* Widget)
{
	FInputModeGameAndUI InputMode;
	InputMode.SetWidgetToFocus(Widget->TakeWidget());
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void AGSBPlayerController::EnterUIControlMode()
{
	bShowMouseCursor = true;

	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::DoNotLock);
	SetInputMode(InputMode);
}

void AGSBPlayerController::ExitUIControlMode()
{
	bShowMouseCursor = false;

	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);
}
