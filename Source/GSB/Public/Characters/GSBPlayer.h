// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GSBCharacter.h"
#include "Interfaces/InteractableActor.h"
#include "Interfaces/ItemPickupActor.h"
#include "GSBPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UGSBInventoryWidget;
class UGSBPlayerOverlay;
class UGSBWindowWidget;
class AGSBPlayerHUD;
class AFacilityBuilder;
class AGSBPlayerController;
class AConstructibleFacility;
struct FInputActionValue;

enum class EGamePlayMode : uint8
{
	EGPM_Combat,
	EGPM_Build
};

enum class EZoomState : uint8
{
	EZS_None,
	EZS_ADS
};

UCLASS()
class GSB_API AGSBPlayer : public AGSBCharacter, public IItemPickupActor
{
	GENERATED_BODY()
public:
	AGSBPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	///////////////////////////////////////////////////////////
	// Inputs
	///////////////////////////////////////////////////////////
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void ToggleCombatAndBuildMode();
	void Interaction();
	void SelectInteractionScroll(const FInputActionValue& Value);
	void ToggleInventory();
	void Esc_Triggered();
	void HideWindow();
	void ConfirmFacilityPlacement();
	void CancelFacilityPreview();
	void SelectFacilityType(uint32 Index);
	virtual void Ability1_Started() {}
	virtual void Ability2_Started() {}
	virtual void Ability3_Started() {}

	// Default Inputs;
	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Default Inputs")
	UInputMappingContext* DefaultInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Default Inputs")
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Default Inputs")
	UInputAction* LookInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Default Inputs")
	UInputAction* JumpInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Default Inputs")
	UInputAction* ToggleCombatAndBuildModeInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Default Inputs")
	UInputAction* InteractionInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Default Inputs")
	UInputAction* SelectInteractionScrollInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Default Inputs")
	UInputAction* ToggleInventoryInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Default Inputs")
	UInputAction* EscInputAction;

	// Build Inputs
	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Build Inputs")
	UInputMappingContext* BuildModeInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Build Inputs")
	UInputAction* ConfirmFacilityPlacementInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Build Inputs")
	UInputAction* CancelFacilityPreviewInputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Build Inputs")
	TArray<UInputAction*> SelectFacilityTypeInputActions;

	// Combat Inputs
	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Combat Inputs")
	UInputMappingContext* CombatModeInputMapping;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Combat Inputs")
	UInputAction* Ability1InputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Combat Inputs")
	UInputAction* Ability2InputAction;

	UPROPERTY(EditDefaultsOnly, Category = "GSBPlayer|Inputs|Combat Inputs")
	UInputAction* Ability3InputAction;

	AGSBPlayerController* PlayerController;

	///////////////////////////////////////////////////////////
	// Components
	///////////////////////////////////////////////////////////
protected:
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	USpringArmComponent* SpringArmComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UCameraComponent* CameraComponent;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UInventoryComponent* InventoryComponent;

	///////////////////////////////////////////////////////////
	// 모드 변경
	///////////////////////////////////////////////////////////
public:
	void SetGamePlayMode(EGamePlayMode NewGamePlayMode);

protected:
	EGamePlayMode GamePlayMode = EGamePlayMode::EGPM_Combat;

private:
	void SetGamePlayMode_Combat();
	void SetGamePlayMode_Build();


	///////////////////////////////////////////////////////////
	// 건설 모드
	///////////////////////////////////////////////////////////
private:
	void UpdateFacilityBuilderLocation();

	UPROPERTY()
	AFacilityBuilder* FacilityBuilder;

	UPROPERTY(EditAnywhere, Category = "GSBPlayer|Build Mode")
	TArray<TSubclassOf<AConstructibleFacility>> TestFacilityClasses;

	///////////////////////////////////////////////////////////
	// Interaction
	///////////////////////////////////////////////////////////
private:
	void UpdateInteractableActor(AActor* Candidate);

	virtual int32 PickupItem(const FItemStack& ItemStack) override;

	AActor* TraceInteractableActor();

	TScriptInterface<IInteractableActor> InteractableActor;

	int32 SelectedInteractionIndex = 0;

	///////////////////////////////////////////////////////////
	// UI
	///////////////////////////////////////////////////////////
protected:

	AGSBPlayerHUD* HUD;

	UGSBPlayerOverlay* OverlayWidget;

	UPROPERTY(EditDefaultsOnly, Category = "AGSBPlayer|UI")
	TSubclassOf<UGSBInventoryWidget> InventoryWidgetClass;

	UGSBWindowWidget* InventoryWindowWidget = nullptr;

	bool IsUIMode() const;

public:
	FORCEINLINE UGSBPlayerOverlay* GetOverlayWidget() const
	{
		return OverlayWidget;
	}
};
