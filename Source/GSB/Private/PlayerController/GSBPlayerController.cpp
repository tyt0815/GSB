// Fill out your copyright notice in the Description page of Project Settings.


#include "PlayerController/GSBPlayerController.h"
#include "PlayerController/GSBPlayerInputActionSetDataAsset.h"
#include "EnhancedInputSubsystems.h"
#include "Components/Widget.h"
#include "DebugHeader.h"

void AGSBPlayerController::BeginPlay()
{
	Super::BeginPlay();
	EnhancedInputLocalPlayerSubsystem = ULocalPlayer::GetSubsystem<UEnhancedInputLocalPlayerSubsystem>(GetLocalPlayer());
	if (!EnhancedInputLocalPlayerSubsystem)
	{
		TRACE_SCREEN_LOG(TEXT("EnhancedInputLocalPlayerSubsystem 캐스팅 실패"));
	}
}

void AGSBPlayerController::ActivateCombatInputContext()
{
	ClearAllInputMappingContext();
	AddInputMappingContext(PlayerInputSet->DefaultInputMapping);
	AddInputMappingContext(PlayerInputSet->InteractionInputMapping);
	AddInputMappingContext(PlayerInputSet->CombatInputMapping);
}

void AGSBPlayerController::ActivateBuildInputContext()
{
	ClearAllInputMappingContext();
	AddInputMappingContext(PlayerInputSet->DefaultInputMapping);
	AddInputMappingContext(PlayerInputSet->InteractionInputMapping);
	AddInputMappingContext(PlayerInputSet->BuildInputMapping);
}

void AGSBPlayerController::AddInputMappingContext(UInputMappingContext* InputMappingContext)
{
	if (!EnhancedInputLocalPlayerSubsystem->HasMappingContext(InputMappingContext))
	{
		EnhancedInputLocalPlayerSubsystem->AddMappingContext(InputMappingContext, 0);
		CurrentInputMappingContexts.Add(InputMappingContext);
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
	if (EnhancedInputLocalPlayerSubsystem->HasMappingContext(InputMappingContext))
	{
		EnhancedInputLocalPlayerSubsystem->RemoveMappingContext(InputMappingContext);
		CurrentInputMappingContexts.Remove(InputMappingContext);
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

void AGSBPlayerController::EnterUIControlMode()
{
	FInputModeGameAndUI InputMode;
	InputMode.SetLockMouseToViewportBehavior(EMouseLockMode::LockInFullscreen);
	SetInputMode(InputMode);

	StoreInputMappingContexts(CurrentInputMappingContexts);
	ClearAllInputMappingContext();
	AddInputMappingContext(PlayerInputSet->DefaultInputMapping);
}

void AGSBPlayerController::ExitUIControlMode()
{
	FInputModeGameOnly InputMode;
	SetInputMode(InputMode);

	ClearAllInputMappingContext();
	AddInputMappingContexts(StoredInputMappingContexts);
}
