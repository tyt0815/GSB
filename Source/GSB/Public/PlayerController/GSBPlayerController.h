// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GSBPlayerController.generated.h"

class AGSBPlayer;
class ATopDownBuildPawn;
class UWidget;
class UInputMappingContext;
class UEnhancedInputLocalPlayerSubsystem;
class UGSBPlayerInputActionSetDataAsset;
class UGSBTopDownBuildPawnInputSet;

enum class EControlledPawn
{
	ECP_GSBPlayer,
	ECP_TopDownBuildPawn
};

enum class EGSBPlayerMode
{
	EPGM_Combat,
	EPGM_Build
};

enum class EInputModeType
{
	EIMT_GameOnly,
	EIMT_UIOnly,
	EIMT_GameAndUI
};

UCLASS()
class GSB_API AGSBPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	virtual void BeginPlay() override;
	
public:
	UEnhancedInputLocalPlayerSubsystem* GetEnhancedInputLocalPlayerSubsystem();

	void AddInputMappingContext(UInputMappingContext* InputMappingContext);

	void AddInputMappingContexts(const TArray<UInputMappingContext*> Contexts);

	void RemoveInputMappingContext(UInputMappingContext* InputMappingContext);

	void ClearAllInputMappingContext();

	void SwitchGamePlayMode_PlayerCombatGameOnly(AGSBPlayer* GSBPlayer);

	void SwitchGamePlayMode_PlayerCombatGameAndUI(AGSBPlayer* GSBPlayer);

	void SwitchGamePlayMode_PlayerBuildGameOnly(AGSBPlayer* GSBPlayer);

	void SwitchGamePlayMode_PlayerBuildGameAndUI(AGSBPlayer* GSBPlayer);

	void SwitchGamePlayMode_TopViewBuild(ATopDownBuildPawn* TopDownBuildPawn);

	void SwitchGamePlayMode_TopViewWindowHandle(ATopDownBuildPawn* TopDownBuildPawn);

	void SetGamePlayMode_PlayerCombatGameOnly();

	void SetGamePlayMode_PlayerCombatGameAndUI();

	void SetGamePlayMode_PlayerBuildGameOnly();

	void SetGamePlayMode_PlayerBuildGameAndUI();

	void SetGamePlayMode_TopViewBuild();

	void SetGamePlayMode_TopViewWindowHandle();

	

	bool IsGSBPlayerControlled() const;

	bool IsTopDownBuildPawnControlled() const;

	bool IsPlayerCombatMode() const;

	bool IsPlayerBuildMode() const;

protected:
	UPROPERTY(EditAnywhere)
	UGSBPlayerInputActionSetDataAsset* InputSet;

private:

	void SetInputMode_GameOnly();

	void SetInputMode_UIOnly();

	void SetInputMode_GameAndUI();

	EControlledPawn ControlledPawnType;

	EGSBPlayerMode GSBPlayerGameMode;

	EInputModeType InputModeType;

public:
	FORCEINLINE UGSBPlayerInputActionSetDataAsset* GetInputSet() const
	{
		return InputSet;
	}
};
