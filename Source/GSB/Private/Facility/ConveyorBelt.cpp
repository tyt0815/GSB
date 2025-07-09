// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ConveyorBelt.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/GridSizeBoxComponent.h"
#include "Components/SplineTransportComponent.h"
#include "Components/ItemReceiveComponent.h"
#include "Components/ItemSendComponent.h"
#include "Items/ItemCrate.h"
#include "DebugHeader.h"

AConveyorBelt::AConveyorBelt()
{
    //TODO: tick 없이 알고리즘 구성 가능할듯
    PrimaryActorTick.bCanEverTick = true;
    TransportComponent = CreateDefaultSubobject<USplineTransportComponent>(TEXT("SplineTransport"));
    TransportComponent->SetupAttachment(GetRootComponent());
    TransportComponent->Deactivate();

    ItemReceiveComponent = CreateDefaultSubobject<UItemReceiveComponent>(TEXT("ItemReceive"));
    ItemSendComponent = CreateDefaultSubobject<UItemSendComponent>(TEXT("ItemSend"));

    FacilityName = TEXT("컨베이어 벨트");
}

void AConveyorBelt::Tick(float DeltaTime)
{
    Super::Tick(DeltaTime);

    ItemReceiveComponent->TryReceiveItem();
    TrySendItem();
}

void AConveyorBelt::CompleteConstruction_Implementation()
{
    Super::CompleteConstruction_Implementation();
    TransportComponent->Activate(false);
    TryAutoConnectToItemSender();
    TryAutoConnectItemReceiver();
}

bool AConveyorBelt::CanReceiveItem() const
{
    return ItemReceiveComponent->CanReceiveItem();
}

void AConveyorBelt::OnConnectedToItemSender(AActor* NewSender)
{
    ItemReceiveComponent->OnConnectedToItemSender(NewSender);
}

void AConveyorBelt::OnDisconnectedFromItemSender()
{
    ItemReceiveComponent->OnDisconnectedFromItemSender();
}

void AConveyorBelt::OnReceiveItem(AActor* Item)
{
    ItemReceiveComponent->OnReceiveItem(Item);
    TransportComponent->TransportActor(Item, TransportComponent->GetSplineLength());
}

FVector AConveyorBelt::GetReceivingDirection() const
{
    return ItemReceiveComponent->GetRecevingDirection();
}

bool AConveyorBelt::TrySendItem()
{
    if (ItemSendComponent->TrySendItem())
    {
        ItemReceiveComponent->TakeReceivedItem();
        return true;
    }
    return false;
}

bool AConveyorBelt::TryConnectItemReceiver(AActor* NewReceiver)
{
    return ItemSendComponent->TryConnectItemReceiver(NewReceiver);
}

void AConveyorBelt::DisconnectItemReceiver()
{
    ItemSendComponent->DisconnectItemReceiver();
}

AActor* AConveyorBelt::GetItemToSend() const
{
    return ItemSendComponent->GetItemToSend();
}

void AConveyorBelt::BeginPlay()
{
    Super::BeginPlay();
    
    ItemReceiveComponent->SetRecevingDirection(TransportComponent->GetStartDirection());
    ItemSendComponent->SetSendingDirection(TransportComponent->GetEndDirection());

    TransportComponent->OnArrived.AddDynamic(this, &AConveyorBelt::SetItemToSend);

    if (DebugPayload)
    {
        OnReceiveItem(DebugPayload);
    }
}

bool AConveyorBelt::TryAutoConnectToItemSender()
{
    if (ItemReceiveComponent->IsConnectedToItemSender())
    {
        return false;
    }
    FVector Start = GetActorLocation();
    FVector End = Start - TransportComponent->GetStartDirection() * 100;
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
        if (ItemReceiveComponent->TryConnectToItemSender(HitResult.GetActor()))
        {
            return true;
        }
    }
    return false;
}

bool AConveyorBelt::TryAutoConnectItemReceiver()
{
    if (ItemSendComponent->IsConnectedToItemReceiver())
    {
        return false;
    }
    FVector Start = GetActorLocation();
    FVector End = Start + TransportComponent->GetEndDirection() * 100;
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
            return true;
        }
    }
    
    return false;
}

void AConveyorBelt::SetItemToSend(AActor* Item)
{
    ItemSendComponent->SetItemToSend(Item);
}
