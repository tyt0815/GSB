// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/Addon/InputPort.h"
#include "Components/ItemReceiveComponent.h"
#include "Components/SplineTransportComponent.h"
#include "Interfaces/InputPortHandler.h"
#include "Kismet/KismetSystemLibrary.h"

AInputPort::AInputPort()
{
	ItemReceiver = CreateDefaultSubobject<UItemReceiveComponent>(TEXT("ItemReceiver"));
}

void AInputPort::OnFacilityLinkedToPowerProvider()
{
    Super::OnFacilityLinkedToPowerProvider();
    AutoConnectItemSender();
}

void AInputPort::CompleteConstruction_Implementation()
{
    Super::CompleteConstruction_Implementation();
    AutoConnectItemSender();
}

void AInputPort::OnConnectedToFacility()
{
    IInputPortHandler* InputPortHandler = Cast<IInputPortHandler>(GetOwner());
    if (InputPortHandler)
    {
        InputPortHandler->RegisterInputPort(this);
    }
}

bool AInputPort::CanReceiveItem() const
{
	return bHasToken;
}

void AInputPort::OnConnectedToItemSender(AActor* NewSender)
{
	ItemReceiver->OnConnectedToItemSender(NewSender);
}

void AInputPort::OnDisconnectedFromItemSender()
{
	ItemReceiver->OnDisconnectedFromItemSender();
}

void AInputPort::OnReceiveItem(AActor* Item)
{
	if (IsValid(Item))
	{
		Item->Destroy();
	}
}

FVector AInputPort::GetReceivingDirection() const
{
	return ItemReceiver->GetRecevingDirection();
}

void AInputPort::BeginPlay()
{
	Super::BeginPlay();

	ItemReceiver->SetRecevingDirection(GetActorForwardVector());
    AutoConnectItemSender();
}

bool AInputPort::TryReceiveItem()
{
	return ItemReceiver->TryReceiveItem();
}

void AInputPort::AutoConnectItemSender()
{
    if (ItemReceiver->IsConnectedToItemSender())
    {
        return;
    }
    FVector Start = GetActorLocation();
    FVector End = Start - GetActorForwardVector() * (GetGridBoxExtent().X + 50);
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
        EDrawDebugTrace::ForDuration,
        HitResults,
        true
    );

    for (const FHitResult& HitResult : HitResults)
    {
        if (ItemReceiver->TryConnectToItemSender(HitResult.GetActor()))
        {
            return;
        }
    }
}

AActor* AInputPort::GetPendingItemFromSender() const
{
	return ItemReceiver->GetPendingItemFromSender();
}