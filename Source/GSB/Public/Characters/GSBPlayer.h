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
class UGSBStorageWindow;
class UGSBInventoryWindow;
class UGSBBuildableFacilityListWindow;
class UGSBStorage;
class UGSBPlayerOverlay;
class AGSBPlayerHUD;
class AGSBPlayerController;
class AConstructibleFacility;
class AFacilityBuilder;
struct FInputActionValue;

UCLASS()
class GSB_API AGSBPlayer : public AGSBCharacter, public IItemPickupActor
{
	GENERATED_BODY()
public:
	AGSBPlayer();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual void PostInitializeComponents() override;

protected:
	virtual void BeginPlay() override;

	///////////////////////////////////////////////////////////
	// Inputs
	///////////////////////////////////////////////////////////
protected:
	void Move(const FInputActionValue& Value);
	void Look(const FInputActionValue& Value);
	void Interaction();
	void SelectInteractionScrollUp();
	void SelectInteractionScrollDown();
	void ToggleInventory();
	void Esc_Triggered();
	void HideWindow();
	void RotatePreview();
	void ConfirmFacilityPlacement();
	void CancelFacilityPreview();
	void ToggleBuildableFacilityList();
	void PreviewConveyorBelt();
	void PreviewExtensionHub();
	void PreviewMiningFacility();
	void SwitchToCombatMode();
	virtual void Ability1_Started() {}
	virtual void Ability2_Started() {}
	virtual void Ability3_Started() {}
	void SwitchToBuildMode();

	AGSBPlayerController* GetPlayerController() const;

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
	bool IsUIMode() const;

	UFUNCTION()
	void OnItemSlotAddedToInventory(UGSBStorageWindow* Storage, UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot);

	AGSBPlayerHUD* GetHUD() const;

	UGSBPlayerOverlay* GetOverlayWidget() const;

	UGSBInventoryWindow* InventoryWindow;

	UGSBBuildableFacilityListWindow* BuildableFacilityListWindow;

public:
	
	FORCEINLINE UInventoryComponent* GetInventoryComponent() const
	{
		return InventoryComponent;
	}
};