// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Blueprint/UserWidget.h"
#include "Interfaces/ChainDeconstrutableFacility.h"
#include "GSBChainDeconstructionButton.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API UGSBChainDeconstructionButton : public UUserWidget
{
	GENERATED_BODY()
public:
	virtual void NativeConstruct() override;
	
private:
	UPROPERTY(meta = (BindWidget))
	class UButton* ChainDeconstructButton;

	UFUNCTION()
	void DeconstructConnectedConveyorChain();

	TScriptInterface<IChainDeconstrutableFacility> ChainDeconstrutableFacility;
public:
	FORCEINLINE void SetChainDeconstrutableFacility(TScriptInterface<IChainDeconstrutableFacility> InChainDeconstrutableFacility)
	{
		ChainDeconstrutableFacility = InChainDeconstrutableFacility;
	}
};
