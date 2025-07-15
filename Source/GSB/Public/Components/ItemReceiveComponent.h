// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "Interfaces/ItemSender.h"
#include "ItemReceiveComponent.generated.h"

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSB_API UItemReceiveComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UItemReceiveComponent();

public:	
	bool CanReceiveItem() const;

	bool TryReceiveItem();

	bool TryConnectToItemSender(AActor* NewSender);

	void DisconnectFromItemSender();

	bool IsConnectedToItemSender() const;

	void OnConnectedToItemSender(AActor* NewSender);

	void OnDisconnectedFromItemSender();

	void OnReceiveItem(AActor* Item);

	AActor* TakeReceivedItem();

	// Sender측에서 보낼 예정인 Item을 조회하는 함수
	AActor* GetPendingItemFromSender() const;

private:
	bool IsValidItemSender(UObject* Object) const;

	UPROPERTY(VisibleAnywhere, Category = "UItemReceiveComponent")
	TScriptInterface<IItemSender> ConnectedItemSender = nullptr;
	
	TWeakObjectPtr<AActor> ReceivedItem;

	// 받는 방향을 나타냄. 받는 방향이란 받는 아이템의 진행방향, 예를 들어 (1, 0, 0) 라고 할때 아이템이 (-1, 0, 0) 와야 받을 수 있음을 뜻함.
	// ZeroVector의 경우 전방향을 뜻한다.
	FVector RecevingDirection = FVector::ZeroVector;

public:
	FORCEINLINE AActor* GetReceivedItem() const
	{
		return ReceivedItem.Get();
	}
	FORCEINLINE FVector GetRecevingDirection() const
	{
		return RecevingDirection;
	}
	FORCEINLINE void SetRecevingDirection(FVector NewDirection)
	{
		RecevingDirection = NewDirection.GetSafeNormal();
	}
	FORCEINLINE TScriptInterface<IItemSender> GetConnectedItemSender() const
	{
		return ConnectedItemSender;
	}
};
