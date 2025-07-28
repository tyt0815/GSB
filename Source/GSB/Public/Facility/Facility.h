// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridSnapActor.h"
#include "Interfaces/InteractableActor.h"
#include "Components/InteractionComponent.h"
#include "HUDs/GSBFacilityDetailWindow.h"
#include "Facility.generated.h"

class AFacilityAddon;
class UMeshOverlayHelperComponent;

UCLASS()
class GSB_API AFacility : public AGridSnapActor, public IInteractableActor
{
	GENERATED_BODY()
	
public:	
	AFacility();

	virtual void Tick(float DeltaSeconds) override final;

	virtual void EndPlay(EEndPlayReason::Type EndPlayReason) override;

	virtual void GetInteractionDescriptions(TArray<FString>& Descriptions) override;

	virtual void Interaction(int32 Index, AActor* Interactor) override;

	virtual void InteractionByDescription(const FString& Description, AActor* Interactor) override;

	virtual int32 GetNumInteractions() const override;

	virtual bool IsInteractable() const override;

	virtual bool IsInteractionListDirty() const override;

	virtual void ClearInteractionListDirtyFlag() override;

	virtual void SetHighlighInteractableActor(bool bVisibility) override;

protected:
	virtual void BeginPlay();

public:
	virtual void ConnectFacilityAddon(AFacilityAddon* Addon);

	virtual bool IsOperating() const { return true; };

	FText GetFacilityName() const;

	FVector GetFacilityMeshTop() const;

protected:
	virtual void AddDefaultInteractions();

	virtual void Tick_OnOperating(float DeltaSecond) {};

	UFUNCTION()
	virtual void OnShowDetailInteraction(AActor* Interactor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	UChildActorComponent* MeshComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMeshOverlayHelperComponent* MeshOverlayHelperComponent;

	UPROPERTY(EditDefaultsOnly, Category = "GSB")
	class UGSBFacilityDataAsset* FacilityData;

	TArray<AFacilityAddon*> ConnectedAddons;

private:
	void AddHighlightTargetRecursive(AActor* Actor);

	//////////////////////////////////////////////////////////////
	// UI
	//////////////////////////////////////////////////////////////
public:
	void UnlinkFacilityDetailWindow();

protected:
	UPROPERTY(EditDefaultsOnly, Category = "UI")
	TSubclassOf<UGSBFacilityDetailWindow> DetailWindowClass;

	UGSBFacilityDetailWindow* DetailWindow;


	//////////////////////////////////////////////////////////////
	// inline 함수
	//////////////////////////////////////////////////////////////
public:
	FORCEINLINE class UGSBFacilityDataAsset* GetFacilityData() const
	{
		return FacilityData;
	}
};
