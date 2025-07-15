// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/Addon/OutputPort.h"
#include "Components/ItemSendComponent.h"
#include "Interfaces/OutputPortHandler.h"
#include "Items/ItemCrate.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

AOutputPort::AOutputPort()
{
	ItemSender = CreateDefaultSubobject<UItemSendComponent>(TEXT("ItemSender"));
}

void AOutputPort::OnFacilityLinkedToPowerProvider()
{
	Super::OnFacilityLinkedToPowerProvider();
	AutoConnectItemReceiver();
}

void AOutputPort::CompleteConstruction_Implementation()
{
	Super::CompleteConstruction_Implementation();
	AutoConnectItemReceiver();
}

void AOutputPort::OnConnectedToFacility()
{
	IOutputPortHandler* OutputPortHandler = Cast<IOutputPortHandler>(GetOwner());
	if (OutputPortHandler)
	{
		OutputPortHandler->RegisterOutputPort(this);
	}
}

bool AOutputPort::TrySendItem()
{
	return ItemSender->TrySendItem();
}

bool AOutputPort::TryConnectItemReceiver(AActor* NewReceiver)
{
	return ItemSender->TryConnectItemReceiver(NewReceiver);
}

void AOutputPort::DisconnectItemReceiver()
{
	ItemSender->DisconnectItemReceiver();
}

AActor* AOutputPort::GetItemToSend() const
{
	return ItemSender->GetItemToSend();
}

void AOutputPort::BeginPlay()
{
	Super::BeginPlay();

	ItemSender->SetSendingDirection(GetActorForwardVector());
}

TScriptInterface<IItemReceiver> AOutputPort::GetConnectedItemReceiver() const
{
	return ItemSender->GetConnectedItemReceiver();
}

bool AOutputPort::IsConnectedToReceiver()
{
	return ItemSender->IsConnectedToItemReceiver();
}

void AOutputPort::AutoConnectItemReceiver()
{
	if (IsConnectedToReceiver())
	{
		return;
	}

	FVector Start = GetActorLocation() + GetActorForwardVector() * (GetGridBoxExtent().X + 25);
	FVector End = Start + GetActorForwardVector() * 25;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
	TArray<AActor*> ActorsToIgnore;
	TArray<FHitResult> HitResults;

	UKismetSystemLibrary::LineTraceMultiForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);

	for (const FHitResult& HitResult : HitResults)
	{
		if (TryConnectItemReceiver(HitResult.GetActor()))
		{
			return;
		}
	}
}

bool AOutputPort::TryCreateAndSendItemCrate(UItemDataAsset* ItemData)
{
	UWorld* World = GetWorld();
	UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance());
	if (GameInst && World)
	{
		if (ItemSender->IsConnectedToItemReceiver() && ItemSender->GetConnectedItemReceiver()->CanReceiveItem())
		{
			FTransform SpawnTransform = GetActorTransform();
			SpawnTransform.SetLocation(SpawnTransform.GetLocation() - FVector(0, 0, 50));
			if (UClass* ItemCrateClass = GameInst->GetActorClass(TEXT("ItemCrate")))
			{
				if (AItemCrate* ItemCrate = World->SpawnActor<AItemCrate>(ItemCrateClass, SpawnTransform))
				{
					ItemCrate->SetItemData(ItemData);
					ItemSender->SetItemToSend(ItemCrate);
					ItemSender->SendItem();
					return true;
				}
			}
			else
			{
				TRACE_SCREEN_LOG(TEXT("ItemCrateClass가 nullptr입니다."))
			}
		}
	}
	return false;
}
