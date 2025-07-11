// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/ItemReceiver.h"
#include "ItemSendComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSB_API UItemSendComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemSendComponent();

	bool IsConnectedToItemReceiver() const;

	bool CanSendItem()  const;

	// Sender와 Receiver 양측 모두 주고 받을 준비가 되어있는지를 확인한다.
	bool CanTransferItem() const;

	void DisconnectItemReceiver();

	void SendItem();

	bool TrySendItem();

	bool TryConnectItemReceiver(AActor* NewReceiver);

private:
	bool IsValidItemReceiver(UObject* Actor) const;

	bool IsDirectionalConnectionValid(const FVector& RecevingDirection) const;

	UPROPERTY(VisibleAnywhere, Category = "UItemSendComponent")
	TScriptInterface<IItemReceiver> ConnectedItemReceiver = nullptr;

	UPROPERTY(VisibleAnywhere, Category = "UItemSendComponent")
	TWeakObjectPtr<AActor> ItemToSend;

	FVector SendingDirection = FVector::ZeroVector;

public:
	FORCEINLINE	TScriptInterface<IItemReceiver> GetConnectedItemReceiver() const
	{
		return ConnectedItemReceiver;
	}
	FORCEINLINE	FVector GetSendingDirection() const
	{
		return SendingDirection;
	}
	FORCEINLINE void SetSendingDirection(FVector NewDirection)
	{
		SendingDirection = NewDirection.GetSafeNormal();
	}
	FORCEINLINE AActor* GetItemToSend() const
	{
		return ItemToSend.Get();
	}
	FORCEINLINE void SetItemToSend(AActor* Item)
	{
		ItemToSend = Item;
	}
};
