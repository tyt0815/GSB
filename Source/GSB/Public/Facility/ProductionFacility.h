// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/PowerDistributorLinkedFacility.h"
#include "Interfaces/InputPortHandler.h"
#include "Interfaces/OutputPortHandler.h"
#include "ProductionFacility.generated.h"

class UItemStorageComponent;
class UProductionRecipeSetDataAsset;
class UItemDataAsset;
class URetryPrioritizedActorRequestHandlerComponent;

struct FItemStack;

UCLASS()
class GSB_API AProductionFacility : public APowerDistributorLinkedFacility, public IInputPortHandler, public IOutputPortHandler
{
	GENERATED_BODY()
public:
	AProductionFacility();

	////////////////////////////////////////////////
	// IInputPortHandler
	////////////////////////////////////////////////
	virtual void RegisterInputPort(AInputPort* InputPort) override;

	////////////////////////////////////////////////
	// IOutputPortHandler
	////////////////////////////////////////////////
	virtual void RegisterOutputPort(AOutputPort* OutputPort) override;
protected:
	virtual void BeginPlay() override;

public:
	void HandleReceivedItem(const FItemStack& ItemStack);

	float GetProductionProgress() const;

	float GetProductionTime() const;

protected:
	struct FProductionRecipe* FindProductionRecipe();
	
	bool CanStartProduce();

	UFUNCTION()
	bool TryReceiveItemFromInputPort(AActor* InputPort);

	UFUNCTION()
	bool TrySendItemToOutputPort(AActor* OutputPort);

	UPROPERTY(VisibleAnywhere)
	UItemStorageComponent* InputStorageComponent;

	UPROPERTY(VisibleAnywhere)
	UItemStorageComponent* OutputStorageComponent;

	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* InputPortHandler;

	UPROPERTY(VisibleAnywhere)
	URetryPrioritizedActorRequestHandlerComponent* OutputPortHandler;

	UPROPERTY(EditAnywhere, Category = "ProductionFacility")
	UProductionRecipeSetDataAsset* RecipeSet;

	const FItemStack* ProducingItem = nullptr;

	FTimerHandle ProducingTimerHandle;

private:
	bool TryStartProduction();

	UFUNCTION()
	void ProduceItem();

	UFUNCTION()
	bool CanReceiveItem(const AInputPort* InputPort);

public:
	FORCEINLINE UItemStorageComponent* GetInputStorageComponent() const
	{
		return InputStorageComponent;
	}
	FORCEINLINE UItemStorageComponent* GetOutputStorageComponent() const
	{
		return OutputStorageComponent;
	}
};
