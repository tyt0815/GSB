// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GSBCharacter.h"
#include "Interfaces/InteractableActor.h"
#include "Interfaces/ItemPickupActor.h"
#include "Interfaces/InventoryInterface.h"
#include "HUDs/GSBPlayerOverlay.h"
#include "GSBPlayer.generated.h"

class UInputMappingContext;
class UInputAction;
class USpringArmComponent;
class UCameraComponent;
class UInventoryComponent;
class UGSBStorageWindow;
class UGSBInventoryWindow;
class UGSBConstructableFacilityListWindow;
class UGSBStorage;
class AGSBPlayerHUD;
class AGSBPlayerController;
class AConstructibleFacility;
class AFacilityBuilder;
struct FInputActionValue;

UCLASS()
class GSB_API AGSBPlayer : public AGSBCharacter, public IItemPickupActor, public IInventoryInterface
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
	void PreviewFacility1();
	void PreviewFacility2();
	void PreviewFacility3();
	void PreviewFacility4();
	void PreviewFacility5();
	void PreviewFacility6();
	void PreviewFacility7();
	void PreviewFacility8();
	void PreviewFacility9();
	void PreviewFacility0();
	void ToggleCombatAndBuildMode();
	void SwitchToTopDownBuildMode();
	void SwitchToCombatMode();
	void SwitchToBuildMode();
	virtual void Ability1_Started() {}
	virtual void Ability2_Started() {}
	virtual void Ability3_Started() {}

	AGSBPlayerController* GetPlayerController() const;

	bool IsControlled() const;

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
	// 전투 모드
	///////////////////////////////////////////////////////////
public:
	void OnEnterCombatMode();

	void OnEnterCombatModeGameOnly();

	void OnEnterCombatModeGameAndUI();

	///////////////////////////////////////////////////////////
	// 건설 모드
	///////////////////////////////////////////////////////////
public:
	void OnEnterBuildMode();
	void OnEnterBuildModeGameOnly();

	void OnEnterBuildModeGameAndUI();


private:
	void UpdateFacilityBuilderLocation();

	UPROPERTY(VisibleAnywhere, Category = "GSB|Build")
	AFacilityBuilder* FacilityBuilder;

	UPROPERTY(VisibleAnywhere, Category = "GSB|Build")
	class ATopDownBuildPawn* TopDownBuildPawn;

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

	UPROPERTY(EditDefaultsOnly, Category = "GSB|UI")
	TSubclassOf<UGSBPlayerOverlay> PlayerOverlayClass;

	UGSBPlayerOverlay* PlayerOverlay;

	UGSBInventoryWindow* InventoryWindow;

public:
	FORCEINLINE virtual UInventoryComponent* GetInventoryComponent() const override
	{
		return InventoryComponent;
	}
	AFacilityBuilder* GetFacilityBuilder() const
	{
		return FacilityBuilder;
	}
};