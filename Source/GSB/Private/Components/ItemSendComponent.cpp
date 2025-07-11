// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemSendComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "DebugHeader.h"

UItemSendComponent::UItemSendComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

}

bool UItemSendComponent::IsConnectedToItemReceiver() const
{
	return IsValidItemReceiver(ConnectedItemReceiver.GetObject());
}

bool UItemSendComponent::CanSendItem() const
{
	return IsConnectedToItemReceiver() && ItemToSend.IsValid();
}

bool UItemSendComponent::CanTransferItem() const
{
	return CanSendItem() && ConnectedItemReceiver->CanReceiveItem();
}

void UItemSendComponent::DisconnectItemReceiver()
{
	if (IsConnectedToItemReceiver())
	{
		ConnectedItemReceiver.GetInterface()->OnDisconnectedFromItemSender();
	}
	ConnectedItemReceiver = nullptr;
}

void UItemSendComponent::SendItem()
{
	ConnectedItemReceiver->OnReceiveItem(ItemToSend.Get());
	ItemToSend = nullptr;
}

bool UItemSendComponent::TrySendItem()
{
	if (CanTransferItem())
	{
		SendItem();
		return true;
	}
	return false;
}

bool UItemSendComponent::TryConnectItemReceiver(AActor* NewReceiver)
{
	if (IsValidItemReceiver(NewReceiver))
	{
		TScriptInterface<IItemReceiver> ItemReceiver = TScriptInterface<IItemReceiver>(NewReceiver);
		if (IsDirectionalConnectionValid(ItemReceiver->GetReceivingDirection()))
		{
			DisconnectItemReceiver();
			ConnectedItemReceiver = ItemReceiver;
			ConnectedItemReceiver.GetInterface()->OnConnectedToItemSender(GetOwner());
			return true;
		}
	}
	return false;
}

bool UItemSendComponent::IsValidItemReceiver(UObject* Actor) const
{
	return IsValid(Actor) && Actor->Implements<UItemReceiver>();
}

bool UItemSendComponent::IsDirectionalConnectionValid(const FVector& RecevingDirection) const
{
	return RecevingDirection.Dot(SendingDirection) > 0.8 ||
		RecevingDirection == FVector::ZeroVector ||
		SendingDirection == FVector::ZeroVector;
}
