// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "Interfaces/InventoryInterface.h"
#include "TopDownBuildPawn.generated.h"

struct FInputActionValue;
class AFacility;

UCLASS()
class GSB_API ATopDownBuildPawn : public APawn, public IInventoryInterface
{
	GENERATED_BODY()

public:
	ATopDownBuildPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

	virtual UInventoryComponent* GetInventoryComponent() const override;

protected:
	virtual void BeginPlay() override;

public:	
	bool IsControlled() const;

	void OnEnterTopViewExploreMode();

	void OnEnterTopViewBuildMode();

	void OnEnterTopViewWindowHandleMode();

protected:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* FloatingPawnMovement;

private:
	void Move(const FInputActionValue& Value);

	void SwitchToThirdPersonBuildMode();
	void ToggleBuildableFacilityList();

	void RotatePreview();
	void ConfirmFacilityPlacement();
	void CancelFacilityPreview();
	void PreviewFacility(int32 Index);
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
	void SelectFacility();

	void GetMouseWorldPosition(FVector& WorldLocation, FVector& WorldDirection);

	void LineTraceSingleToMouseDown(
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		const TArray<AActor*>& ActorsToIgnore,
		FHitResult& OutHit
	);

	void LineTraceMultiToMouseDown(
		const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
		const TArray<AActor*>& ActorsToIgnore,
		TArray<FHitResult>& OutHits
	);

	void OpenFacilityInteractionContextMenu(AFacility* Facility);

	UFUNCTION()
	void HandleOnFacilityInteractionContextMenuClosed(class UGSBContextMenu* Menu);

	UFUNCTION()
	void HandleOnFacilityInteractionContextMenuEntryClicked(class UGSBContextMenuEntry* Entry);

	void UpdateFacilityBuilderLocation();

	class AGSBPlayer* OwningPlayer;

	class AFacilityBuilder* FacilityBuilder;

	class UGSBPlayerOverlay* PlayerOverlay;	

public:
	FORCEINLINE void SetOwningPlayer(class AGSBPlayer* NewPlayer)
	{
		OwningPlayer = NewPlayer;
	}
	FORCEINLINE void SetFacilityBuilder(class AFacilityBuilder* NewFacilityBuilder)
	{
		FacilityBuilder = NewFacilityBuilder;
	}
	FORCEINLINE void SetPlayerOverlay(class UGSBPlayerOverlay* NewPlayerOverlay)
	{
		PlayerOverlay = NewPlayerOverlay;
	}

};
