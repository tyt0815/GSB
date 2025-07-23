// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/Facility.h"
#include "ConstructibleFacility.generated.h"

UCLASS()
class GSB_API AConstructibleFacility : public AFacility
{
	GENERATED_BODY()

public:
	AConstructibleFacility();

	virtual bool IsOperating() const override;

protected:
	virtual void AddDefaultInteractions() override;

protected:
	virtual void BeginPlay() override;

public:
	bool IsConstructed() const;

	bool IsConstructing() const;

	bool IsDeconstructing() const;

	UFUNCTION(BlueprintPure, Category = "AConstructibleFacility|Construction")
	float GetConstructionProgress() const;

	UFUNCTION(BlueprintPure, Category = "AConstructibleFacility|Construction")
	float GetDeconstructionProgress() const;

	bool TryBeginConstruction();

	bool TryBeginDeconstruction();

protected:
	void CreateAllDynamicMaterialInstances();

	TArray<UMaterialInstanceDynamic*> AllDynamicMaterialInstances;

	UFUNCTION()
	void HandleCancelConstruction(AActor* Interactor);

	UFUNCTION()
	void HandleDeconstructRequest(AActor* Interactor);

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTimelineComponent* ConstructionTimeline;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly)
	class UTimelineComponent* DeconstructionTimeline;

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void BeginConstruction();
	virtual void BeginConstruction_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void BeginDeconstruction();
	virtual void BeginDeconstruction_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void OnConstructing(float Progress);
	virtual void OnConstructing_Implementation(float Progress);

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void OnDeconstructing(float Progress);
	virtual void OnDeconstructing_Implementation(float Progress);

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void CompleteConstruction();
	virtual void CompleteConstruction_Implementation();	

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Construction")
	void CompleteDeconstruction();
	virtual void CompleteDeconstruction_Implementation();

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AConstructibleFacility|Construction")
	float ConstructionTime = 2;

	UPROPERTY(EditDefaultsOnly, BlueprintReadOnly, Category = "AConstructibleFacility|Construction")
	float DeconstructionTime = 2;

private:
	void InitializeConstructionTimeline();

	void InitializeDeconstructionTimeline();

	void SetDissolveEffect(float Amount);
};
