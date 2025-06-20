// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Facility.h"
#include "ConstructibleFacility.generated.h"

enum class EFacilityBuildType
{
	EFBT_Single,
	EFBT_ChainLinear
};

UENUM(BlueprintType)
enum class EFacilityState : uint8
{
	EFS_None			= 0,
	EFS_Preview			= 1							UMETA(DisplayName = "Preview"),
	EFS_Constructing	= EFS_Preview << 1			UMETA(DisplayName = "Constructing"),
	EFS_Running			= EFS_Constructing << 1 	UMETA(DisplayName = "Running"),
	EFS_Waiting			= EFS_Running << 1			UMETA(DisplayName = "Waiting"),
	EFS_PowerOff		= EFS_Waiting << 1			UMETA(DisplayName = "PowerOff")
};

ENUM_CLASS_FLAGS(EFacilityState);

UCLASS()
class GSB_API AConstructibleFacility : public AFacility
{
	GENERATED_BODY()
public:
	virtual void Tick(float Delta) override;

	////////////////////////////////////////////////////////////
	// Facility State Transition
	////////////////////////////////////////////////////////////

public:
	bool TrySetFacilityState(const EFacilityState& NewFacilityState);

protected:
	bool TryEnterPreviewState();
	bool TryEnterConstructingState();
	bool TryEnterRunningState();
	bool TryEnterWaitingState();
	bool TryEnterPowerOffState();
	virtual bool CanEnterPreviewState();
	virtual bool CanEnterConstructingState();
	virtual bool CanEnterRunningState();
	virtual bool CanEnterWaitingState();
	virtual bool CanEnterPowerOffState();

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility")
	void OnEnterPreviewState();
	virtual void OnEnterPreviewState_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility")
	void OnEnterConstructingState();
	virtual void OnEnterConstructingState_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility")
	void OnEnterRunningState();
	virtual void OnEnterRunningState_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility")
	void OnEnterWaitingState();
	virtual void OnEnterWaitingState_Implementation();

	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility")
	void OnEnterPowerOffState();
	virtual void OnEnterPowerOffState_Implementation();

	EFacilityState FacilityState = EFacilityState::EFS_Waiting;

private:

	////////////////////////////////////////////////////////////
	// On Preview State
	////////////////////////////////////////////////////////////
public:
	bool IsValidPlace();


	////////////////////////////////////////////////////////////
	// On Constructing State
	////////////////////////////////////////////////////////////
public:
	UFUNCTION(BlueprintPure, Category = "AConstructibleFacility|Constructing")
	float GetConstructionProgress() const;

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Constructing")
	void Tick_Constructing(float Delta);
	virtual void Tick_Constructing_Implementation(float Delta);

	UPROPERTY(BlueprintReadOnly, Category = "AConstructibleFacility|Constructing")
	float ConstructionTime = 2;

	UPROPERTY(BlueprintReadOnly, Category = "AConstructibleFacility|Constructing")
	float ElapsedConstructionTime = 0;

	////////////////////////////////////////////////////////////
	// On Running State
	////////////////////////////////////////////////////////////

protected:
	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Running")
	void Tick_Running(float Delta);
	virtual void Tick_Running_Implementation(float Delta);

	////////////////////////////////////////////////////////////
	// On Waiting State
	////////////////////////////////////////////////////////////
	UFUNCTION(BlueprintNativeEvent, Category = "AConstructibleFacility|Waiting")
	void Tick_Waiting(float Delta);
	virtual void Tick_Waiting_Implementation(float Delta);

	////////////////////////////////////////////////////////////
	// On PowerOff State
	////////////////////////////////////////////////////////////

public:
	FORCEINLINE EFacilityState GetFacilityState() const
	{
		return FacilityState;
	}
};
