// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ConveyorBelt.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/GridSizeBoxComponent.h"
#include "Components/SplineTransportComponent.h"
#include "Components/ItemReceiveComponent.h"
#include "Components/ItemSendComponent.h"
#include "Items/ItemCrate.h"
#include "Items/DroppedItem.h"
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
}

void AConveyorBelt::Tick_OnOperating(float DeltaTime)
{
    Super::Tick_OnOperating(DeltaTime);

    ItemReceiveComponent->TryReceiveItem();
    TrySendItem();
}

void AConveyorBelt::CompleteConstruction_Implementation()
{
    Super::CompleteConstruction_Implementation();
    TransportComponent->Activate();
    TryAutoConnectToItemSender();
    TryAutoConnectItemReceiver();
    
    if (DebugPayload)
    {
        OnReceiveItem(DebugPayload);
    }

    ItemReceiveComponent->TryReceiveItem();
}

bool AConveyorBelt::CanReceiveItem() const
{
    return IsOperating() && !IsTransporting();
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

void AConveyorBelt::DeconstructConnectedConveyorChain()
{
    DeconstructConnectedReceiverConveyorChain(false);
    DeconstructConnectedSenderConveyorChain(false);
    TryBeginDeconstruction();
}

void AConveyorBelt::DeconstructConnectedReceiverConveyorChain(bool bDeconstructSelf)
{
    if (AConveyorBelt* Receiver = Cast<AConveyorBelt>(ItemSendComponent->GetConnectedItemReceiver().GetObject()))
    {
        Receiver->DeconstructConnectedReceiverConveyorChain(true);
    }
    if (bDeconstructSelf)
    {
        TryBeginDeconstruction();
    }
}

void AConveyorBelt::DeconstructConnectedSenderConveyorChain(bool bDeconstructSelf)
{
    if (AConveyorBelt* Sender = Cast<AConveyorBelt>(ItemReceiveComponent->GetConnectedItemSender().GetObject()))
    {
        Sender->DeconstructConnectedSenderConveyorChain(true);
    }
    if (bDeconstructSelf)
    {
        TryBeginDeconstruction();
    }
}

bool AConveyorBelt::IsTransporting() const
{
    return IsValid(ItemReceiveComponent->GetReceivedItem()) ||
        TransportComponent->IsTransporting() ||
        IsValid(ItemSendComponent->GetItemToSend())
        ;
}

void AConveyorBelt::BeginPlay()
{
    Super::BeginPlay();
    
    ItemReceiveComponent->SetRecevingDirection(TransportComponent->GetStartDirection());
    ItemSendComponent->SetSendingDirection(TransportComponent->GetEndDirection());

    TransportComponent->OnArrived.AddDynamic(this, &AConveyorBelt::SetItemToSend);
}

void AConveyorBelt::BeginDeconstruction_Implementation()
{
    if (AItemCrate* ItemCrate = Cast<AItemCrate>(ItemReceiveComponent->GetReceivedItem()))
    {
        ItemCrate->ConvertToDroppedItem();
    }
    if (AItemCrate* ItemCrate = Cast<AItemCrate>(ItemSendComponent->GetItemToSend()))
    {
        ItemCrate->ConvertToDroppedItem();
    }
    for (AActor* Actor : TransportComponent->GetTransportedActors())
    {
        if (AItemCrate* ItemCrate = Cast<AItemCrate>(Actor))
        {
            ItemCrate->ConvertToDroppedItem();
        }
    }

    ItemReceiveComponent->DisconnectFromItemSender();
    ItemSendComponent->DisconnectItemReceiver();

    Super::BeginDeconstruction_Implementation();
}

bool AConveyorBelt::TryAutoConnectToItemSender()
{
    if (ItemReceiveComponent->IsConnectedToItemSender())
    {
        return false;
    }
    FVector Start = GetActorLocation() - TransportComponent->GetStartDirection() * 75;
    FVector End = Start - TransportComponent->GetStartDirection() * 25;
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
    FVector Start = GetActorLocation() + TransportComponent->GetEndDirection() * 75;
    FVector End = Start + TransportComponent->GetEndDirection() * 25;
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
