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
class AGSBPlayerController;
class AConstructibleFacility;
class AFacilityBuilder;
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
	void SelectInteractionScrollUp();
	void SelectInteractionScrollDown();
	void ToggleInventory();
	void Esc_Triggered();
	void HideWindow();
	void RotatePreview();
	void ConfirmFacilityPlacement();
	void CancelFacilityPreview();
	void PreviewConveyorBelt();
	void PreviewExtensionHub();
	void PreviewMiningFacility();
	virtual void Ability1_Started() {}
	virtual void Ability2_Started() {}
	virtual void Ability3_Started() {}

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

	AFacilityBuilder* FacilityBuilder;

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


private:
	UFUNCTION()
	void SetOverlayWidget();

public:
	FORCEINLINE UGSBPlayerOverlay* GetOverlayWidget() const
	{
		return OverlayWidget;
	}
};
