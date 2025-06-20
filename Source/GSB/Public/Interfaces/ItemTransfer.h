// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "ItemTransfer.generated.h"

class AItemCrate;
class USplineComponent;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UItemTransfer : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GSB_API IItemTransfer
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:

	virtual USplineComponent* GetSplineComponent() const = 0;

	virtual AItemCrate* GetPayload() const = 0;

	virtual void SetPayload(AItemCrate* ItemCrate) = 0;

protected:
	virtual void InitializeSplineSpeed() = 0;

public:
	FVector GetItemReceiveDirection() const;

	FVector GetItemSendDirection() const;

	bool CanSendItem() const;

	bool CanReceiveItem() const;

	bool CanConnectItemReceiver(IItemTransfer* Receiver) const;

	// TryOutputItemTo에서 호출하기 위해 만들어진 함수.
	// 사용에 주의를 요함.
	void ReceiveItem(AItemCrate* Item);

	bool TrySetItemSender(UObject* NewSender);

	bool TrySetItemSender(IItemTransfer* NewSender);

	bool TrySetItemReceiver(UObject* NewReceiver);

	bool TrySetItemReceiver(IItemTransfer* NewReceiver);

	void DisconnectItemReceiver();

	void DisconnectFromItemSender();

protected:
	bool TryReceiveItemFrom(IItemTransfer* Source);

	bool TrySendItemTo(IItemTransfer* Target);

	void Tick_ItemAlongSpline(float Delta);

	void TraceItemTransferActors(
		UObject* WorldContextObject,
		const FVector& Start,
		const FVector& End,
		const TArray<AActor*>& ActorsToIgnore,
		TArray<FHitResult>& OutHits
	);

	void TraceItemSenderInInputDirection(TArray<FHitResult>& OutHits);

	void TraceItemReceiverInOutputDirection(TArray<FHitResult>& OutHits);

	bool TryConnectItemReceiver();

	bool TryConnectToItemSender();

private:
	float CurrentSplineDistance = 0.0f;

	// 초당 Payload의 이동속도
	float SplineMoveSpeed = 0;

	TScriptInterface<IItemTransfer> ItemSender;

	TScriptInterface<IItemTransfer> ItemReceiver;

public:
	FORCEINLINE void SetSplineMoveSpeed(float Value)
	{
		SplineMoveSpeed = Value;
	}
	FORCEINLINE const TScriptInterface<IItemTransfer>& GetItemSender() const
	{
		return ItemSender;
	}
	FORCEINLINE void SetItemSender(TScriptInterface<IItemTransfer> NewSender)
	{
		ItemSender = NewSender;
	}
	FORCEINLINE const TScriptInterface<IItemTransfer>& GetItemReceiver() const
	{
		return ItemReceiver;
	}
	FORCEINLINE void SetItemReceiver(TScriptInterface<IItemTransfer> NewReceiver)
	{
		ItemReceiver = NewReceiver;
	}
	FORCEINLINE bool IsConnectToItemReceiver() const
	{
		return IsValid(GetItemReceiver().GetObject());
	}
	FORCEINLINE	bool IsConnectToItemSender() const
	{
		return IsValid(GetItemSender().GetObject());
	}
};
