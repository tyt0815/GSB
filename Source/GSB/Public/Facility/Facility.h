// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "BuildSystem/GridSnapActor.h"
#include "Interfaces/InteractableActor.h"
#include "Components/InteractionComponent.h"
#include "HUDs/GSBWindowBody.h"
#include "Facility.generated.h"

class AFacilityAddon;

UCLASS()
class GSB_API AFacility : public AGridSnapActor, public IInteractableActor
{
	GENERATED_BODY()
	
public:	
	AFacility();

	virtual void Tick(float DeltaSeconds) override final;

	virtual void GetInteractionDescriptions(TArray<FString>& Descriptions) override;

	virtual void Interaction(int32 Index, AActor* Interactor) override;

	virtual int32 GetNumInteractions() const override;

protected:
	virtual void BeginPlay();

public:
	void ConnectFacilityAddon(AFacilityAddon* Addon);

	virtual bool IsOperating() const { return true; };

protected:
	virtual void Tick_OnOperating(float DeltaSecond) {};

	UFUNCTION()
	virtual void OnShowDetailInteraction(AActor* Interactor);

	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(EditAnywhere)
	FName FacilityName = FName(TEXT("FacilityName_None"));

	UPROPERTY(EditDefaultsOnly, Category = "AFacility|UI")
	TSubclassOf<UGSBWindowBody> DetailWindowBodyClass;

	UGSBWindowWidget* DetailWindow = nullptr;

	UGSBWindowBody* DetailWindowBody = nullptr;

	void AttachDetailWindowHead(class UGSBWindowHead* Head);

	TArray<AFacilityAddon*> ConnectedAddons;
};
