// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "InteractionComponent.generated.h"

DECLARE_DYNAMIC_MULTICAST_DELEGATE_OneParam(FComponentOnInteractionSignature, AActor*, Interactor);

struct FInteractionData
{
	TArray<FString> InteractionDescriptions;

	TArray<FComponentOnInteractionSignature> OnInteractionDelegates;
};

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSB_API UInteractionComponent : public UBoxComponent
{
	GENERATED_BODY()

public:	
	UInteractionComponent();
	// virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

protected:
	// virtual void BeginPlay() override;

public:
	void Interaction(int32 Index, AActor* Interactor);

	int32 GetInteractionIndex(const FString& Description);

	FComponentOnInteractionSignature& AddInteraction_Internal(const FString& Description);

	void RemoveInteractionAt(int32 i);

	void ClearInteractions();

	void ActivateInteraction();

	void DeactivateInteraction();

	

private:
	FInteractionData InteractionData;

	bool bInteractable = true;

	bool bInteractionListDirty = true;

public:
	FORCEINLINE void GetInteractionDescriptions(TArray<FString>& InDescriptions) const
	{
		InDescriptions = InteractionData.InteractionDescriptions;
	}
	FORCEINLINE int32 GetNumInteractions() const
	{
		return InteractionData.InteractionDescriptions.Num();
	}
	FORCEINLINE void SetDescriptionAt(int32 Index, const FString& Description)
	{
		InteractionData.InteractionDescriptions[Index] = Description;
	}
	FORCEINLINE bool IsInteractable() const
	{
		return bInteractable;
	}
	FORCEINLINE void ClearInteractionListDirtyFlag()
	{
		bInteractionListDirty = false;
	}
	FORCEINLINE bool IsInteractionListDirty() const
	{
		return bInteractionListDirty;
	}
};

#define AddInteractionDynamic(Description, UserObject, FuncName) AddInteraction_Internal(Description).AddDynamic(UserObject, FuncName);