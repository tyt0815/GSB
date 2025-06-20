// Fill out your copyright notice in the Description page of Project Settings.


#include "Interfaces/ItemTransfer.h"
#include "Items/ItemCrate.h"
#include "Components/SplineComponent.h"
#include "Kismet/KismetSystemLibrary.h"

// Add default functionality here for any IItemTransfer functions that are not pure virtual.

FVector IItemTransfer::GetItemReceiveDirection() const
{
    return GetSplineComponent()->GetRotationAtSplinePoint(0, ESplineCoordinateSpace::World)
        .RotateVector(FVector::ForwardVector);
}

FVector IItemTransfer::GetItemSendDirection() const
{
    return GetSplineComponent()->GetRotationAtSplinePoint(GetSplineComponent()->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World)
        .RotateVector(FVector::ForwardVector);
}

bool IItemTransfer::CanSendItem() const
{
    return IsValid(GetPayload()) && CurrentSplineDistance >= GetSplineComponent()->GetSplineLength();
}

bool IItemTransfer::CanReceiveItem() const
{
    return !IsValid(GetPayload());
}

bool IItemTransfer::CanConnectItemReceiver(IItemTransfer* Receiver) const
{
    return Receiver && Receiver->GetItemReceiveDirection().Dot(GetItemSendDirection()) > 0.5f;
}

void IItemTransfer::ReceiveItem(AItemCrate* Item)
{
    SetPayload(Item);
    CurrentSplineDistance = 0;
}

bool IItemTransfer::TrySetItemSender(UObject* NewSender)
{
    if (IsValid(NewSender) && NewSender->Implements<UItemTransfer>())
    {
        SetItemSender(NewSender);
        return true;
    }
    return false;
}

bool IItemTransfer::TrySetItemSender(IItemTransfer* NewSender)
{
    return TrySetItemSender(Cast<UObject>(NewSender));
}

bool IItemTransfer::TrySetItemReceiver(UObject* NewReceiver)
{
    if (IsValid(NewReceiver) && NewReceiver->Implements<UItemTransfer>())
    {
        SetItemReceiver(NewReceiver);
        return true;
    }
    return false;
}

bool IItemTransfer::TrySetItemReceiver(IItemTransfer* NewReceiver)
{
    return TrySetItemReceiver(Cast<UObject>(NewReceiver));
}

void IItemTransfer::DisconnectItemReceiver()
{
    if (IsConnectToItemReceiver())
    {
        GetItemReceiver().GetInterface()->SetItemSender(nullptr);
        SetItemReceiver(nullptr);
    }
}

void IItemTransfer::DisconnectFromItemSender()
{
    if(IsConnectToItemSender())
    {
        GetItemSender().GetInterface()->DisconnectItemReceiver();
    }
}

bool IItemTransfer::TryReceiveItemFrom(IItemTransfer* Source)
{
    if (CanReceiveItem() && Source && Source->CanSendItem())
    {
        return Source->TrySendItemTo(this);
    }
    return false;
}

bool IItemTransfer::TrySendItemTo(IItemTransfer* Target)
{
    if (Target && Target->CanReceiveItem() && CanSendItem())
    {
        Target->ReceiveItem(GetPayload());
        SetPayload(nullptr);
        CurrentSplineDistance = 0;
        return true;
    }
    return false;
}

void IItemTransfer::Tick_ItemAlongSpline(float Delta)
{
    AItemCrate* ItemCrate = GetPayload();
    if (IsValid(ItemCrate))
    {
        USplineComponent* Spline = GetSplineComponent();
        check(ItemCrate);
        check(Spline);
        float SplineLength = Spline->GetSplineLength();
        if (CurrentSplineDistance < SplineLength)
        {
            CurrentSplineDistance += Delta * SplineMoveSpeed;
            CurrentSplineDistance = FMath::Clamp(CurrentSplineDistance, 0, SplineLength);
            ItemCrate->SetActorLocation(
                Spline->GetLocationAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World)
            );
            ItemCrate->SetActorRotation(
                Spline->GetRotationAtDistanceAlongSpline(CurrentSplineDistance, ESplineCoordinateSpace::World)
            );
        }
    }
}

void IItemTransfer::TraceItemTransferActors(
    UObject* WorldContextObject,
    const FVector& Start,
    const FVector& End,
    const TArray<AActor*>& ActorsToIgnore,
    TArray<FHitResult>& OutHits
)
{
    TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
    ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel4));
    UKismetSystemLibrary::LineTraceMultiForObjects(
        WorldContextObject,
        Start,
        End,
        ObjectTypes,
        false,
        ActorsToIgnore,
        EDrawDebugTrace::ForOneFrame,
        OutHits,
        true
    );
}

void IItemTransfer::TraceItemSenderInInputDirection(TArray<FHitResult>& OutHits)
{
    AActor* ThisActor = Cast<AActor>(this);
    if (!IsValid(ThisActor))
    {
        return;
    }
    FVector Start = ThisActor->GetActorLocation();
    FVector End = Start - ThisActor->GetActorForwardVector() * 60.0f;
    TArray<AActor*> ActorsToIgnore;
    TraceItemTransferActors(ThisActor, Start, End, ActorsToIgnore, OutHits);
}

void IItemTransfer::TraceItemReceiverInOutputDirection(TArray<FHitResult>& OutHits)
{
    AActor* ThisActor = Cast<AActor>(this);
    if (!IsValid(ThisActor))
    {
        return;
    }
    FVector Start = ThisActor->GetActorLocation();
    FVector Direction = 
        GetSplineComponent()->GetRotationAtSplinePoint(GetSplineComponent()->GetNumberOfSplinePoints() - 1, ESplineCoordinateSpace::World)
        .RotateVector(FVector::ForwardVector);
    FVector End = Start + Direction * 60;
    TArray<AActor*> ActorsToIgnore;
    TraceItemTransferActors(ThisActor, Start, End, ActorsToIgnore, OutHits);
}

bool IItemTransfer::TryConnectItemReceiver()
{
    if (IsConnectToItemReceiver())
    {
        return false;
    }

    TArray<FHitResult> HitResults;
    TraceItemReceiverInOutputDirection(HitResults);

    for (const FHitResult& HitResult : HitResults)
    {
        IItemTransfer* NewItemReceiver = Cast<IItemTransfer>(HitResult.GetActor());
        if (CanConnectItemReceiver(NewItemReceiver))
        {
            DisconnectItemReceiver();
            NewItemReceiver->DisconnectFromItemSender();
            if (TrySetItemReceiver(HitResult.GetActor()))
            {
                return NewItemReceiver->TrySetItemSender(this);
            }
        }
    }
    return false;
}

bool IItemTransfer::TryConnectToItemSender()
{
    if (IsConnectToItemSender())
    {
        return false;
    }

    TArray<FHitResult> HitResults;
    TraceItemSenderInInputDirection(HitResults);

    for (const FHitResult& HitResult : HitResults)
    {
        IItemTransfer* NewItemSender = Cast<IItemTransfer>(HitResult.GetActor());
        if (NewItemSender && NewItemSender->CanConnectItemReceiver(this))
        {
            DisconnectFromItemSender();
            NewItemSender->DisconnectItemReceiver();
            if (TrySetItemSender(HitResult.GetActor()))
            {
                return NewItemSender->TrySetItemReceiver(this);
            }
        }
    }
    return false;
}
