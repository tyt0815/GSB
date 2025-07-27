// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Interfaces/InteractableActor.h"
#include "Items/ItemDataAsset.h"
#include "DroppedItem.generated.h"

class UInteractionComponent;
class UItemDataAsset;
class UBoxComponent;
class UNiagaraSystem;

UCLASS()
class GSB_API ADroppedItem : public AActor, public IInteractableActor
{
	GENERATED_BODY()
	
public:	
	ADroppedItem();

	virtual void GetInteractionDescriptions(TArray<FString>& Descriptions) override;

	virtual void Interaction(int32 Index, AActor* Interactor) override;

	virtual void InteractionByDescription(const FString& Description, AActor* Interactor) override;

	virtual int32 GetNumInteractions() const override;

	virtual bool IsInteractable() const override;

	virtual void SetHighlighInteractableActor(bool bHighlight) override;

	virtual bool IsInteractionListDirty() const override;

	virtual void ClearInteractionListDirtyFlag() override;

protected:
	virtual void BeginPlay() override;

public:
	static ADroppedItem* CreateDroppedItem(UObject* WorldContext, UItemDataAsset* ItemData, FVector Location);

	static ADroppedItem* CreateDroppedItem(UObject* WorldContext, const struct FItemStack& ItemStack, FVector Location);

	void UpdateItem(const FItemStack& NewItemStack);

protected:
	UPROPERTY(VisibleAnywhere)
	UBoxComponent* ItemOverlapBox;

	UPROPERTY(VisibleAnywhere)
	UInteractionComponent* InteractionComponent;

	UPROPERTY(VisibleAnywhere)
	class UNiagaraComponent* NiagaraComponent;

	UPROPERTY(EditAnywhere, Category = "ADroppedItem")
	FItemStack ItemStack;

	UNiagaraSystem* DefaultNiagara;

	UPROPERTY(EditAnywhere, Category = "ADroppedItem")
	UNiagaraSystem* HighlightedNiagara;

private:
	UFUNCTION()
	void HandlePickup(AActor* Interactor);

	FName ItemName;

public:
	FORCEINLINE FName GetItemName() const
	{
		return ItemName;
	}
};
