// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridSnapActor.h"
#include "Interfaces/InteractableActor.h"
#include "Facility.generated.h"

class AFacilityAddon;
class UInteractionComponent;
class UGSBFacilityDetailWidget;

UCLASS()
class GSB_API AFacility : public AGridSnapActor, public IInteractableActor
{
	GENERATED_BODY()
	
public:	
	AFacility();

	virtual void GetInteractionDescriptions(TArray<FString>& Descriptions) override;

	virtual void Interaction(int32 Index, AActor* Interactor) override;

	virtual int32 GetNumInteractions() const override;

protected:
	virtual void BeginPlay();

public:
	void ConnectFacilityAddon(AFacilityAddon* Addon);

protected:
	UFUNCTION()
	virtual void OnShowDetailInteraction(AActor* Interactor);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere)
	FName FacilityName = FName(TEXT("FacilityName_None"));

	UPROPERTY(EditDefaultsOnly, Category = "AFacility|UI")
	TSubclassOf<UGSBFacilityDetailWidget> DetailWidgetClass;

	UGSBFacilityDetailWidget* DetailWidget = nullptr;

	TArray<AFacilityAddon*> ConnectedAddons;
};
