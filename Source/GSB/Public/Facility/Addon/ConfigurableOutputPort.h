// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Facility/Addon/OutputPort.h"
#include "Interfaces/InteractableActor.h"
#include "ConfigurableOutputPort.generated.h"

class UInteractionComponent;
class UItemStorageComponent;
class UMeshOverlayHelperComponent;
class UGSBOutputPortDetailWidget;

UCLASS()
class GSB_API AConfigurableOutputPort : public AOutputPort, public IInteractableActor
{
	GENERATED_BODY()

public:
	AConfigurableOutputPort();

	virtual void GetInteractionDescriptions(TArray<FString>& Descriptions) override;

	virtual void Interaction(int32 Index, AActor* Interactor) override;

	virtual int32 GetNumInteractions() const override;

	virtual bool IsInteractable() const override;

	virtual void SetHighlighInteractableActor(bool bVisibility) override;

protected:
	virtual void BeginPlay() override;

public:
	void UpdateSelectedItem(UItemDataAsset* NewData);
	
protected:
	UPROPERTY(EditAnywhere, Category = "AConfigurableOutputPort|ForDebug")
	UItemDataAsset* SelectedItem;

	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite)
	UMeshOverlayHelperComponent* MeshOverlayHelperComponent;

	UPROPERTY(EditDefaultsOnly, Category = "AConfigurableOutputPort")
	TSubclassOf<UGSBOutputPortDetailWidget> DetailWidgetClass;

	TWeakObjectPtr<UGSBOutputPortDetailWidget> DetailWidget;
	
	TWeakObjectPtr<UItemStorageComponent> LinkedStorage;

private:
	UFUNCTION()
	void OnViewDetailInteraction(AActor* Interactor);

public:
	FORCEINLINE UItemDataAsset* GetSelectedItem() const
	{
		return SelectedItem;
	}
};
