// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/ItemReceiveComponent.h"
#include "DebugHeader.h"

UItemReceiveComponent::UItemReceiveComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

bool UItemReceiveComponent::CanReceiveItem() const
{
	return IsConnectedToItemSender() && !ReceivedItem.IsValid();
}

bool UItemReceiveComponent::TryReceiveItem()
{
	if (IsConnectedToItemSender())
	{
		return ConnectedItemSender->TrySendItem();
	}
	return false;
}

bool UItemReceiveComponent::TryConnectToItemSender(AActor* NewSender)
{
	if (IsValidItemSender(NewSender))
	{
		return Cast<IItemSender>(NewSender)->TryConnectItemReceiver(GetOwner());
	}
	return false;
}

void UItemReceiveComponent::DisconnectFromItemSender()
{
	if (IsConnectedToItemSender())
	{
		ConnectedItemSender->DisconnectItemReceiver();
	}
}

bool UItemReceiveComponent::IsConnectedToItemSender() const
{
	return IsValidItemSender(ConnectedItemSender.GetObject());
}

void UItemReceiveComponent::OnConnectedToItemSender(AActor* NewSender)
{
	DisconnectFromItemSender();
	ConnectedItemSender = TScriptInterface<IItemSender>(NewSender);
}

void UItemReceiveComponent::OnDisconnectedFromItemSender()
{
	ConnectedItemSender = nullptr;
}

void UItemReceiveComponent::OnReceiveItem(AActor* Item)
{
	ReceivedItem = Item;
}

AActor* UItemReceiveComponent::TakeReceivedItem()
{
	AActor* Item = ReceivedItem.Get();
	ReceivedItem = nullptr;
	return Item;
}

AActor* UItemReceiveComponent::GetPendingItemFromSender() const
{
	if (IsConnectedToItemSender())
	{
		return ConnectedItemSender->GetItemToSend();
	}
	return nullptr;
}

bool UItemReceiveComponent::IsValidItemSender(UObject* Object) const
{
	return IsValid(Object) && Object->Implements<UItemSender>();
}
