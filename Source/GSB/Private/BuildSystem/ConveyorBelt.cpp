// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/ConveyorBelt.h"
#include "Kismet/KismetSystemLibrary.h"
#include "Components/SplineComponent.h"
#include "Items/ItemCrate.h"
#include "DebugHeader.h"

AConveyorBelt::AConveyorBelt()
{
    SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
    SplineComponent->SetupAttachment(GetRootComponent());
    StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
    StaticMeshComponent->SetupAttachment(SplineComponent);
}

void AConveyorBelt::BeginPlay()
{
    Super::BeginPlay();

    InitializeSplineSpeed();
    TrySetFacilityState(EFacilityState::EFS_Waiting);
}

void AConveyorBelt::BeginDestroy()
{
    Super::BeginDestroy();
}

void AConveyorBelt::OnEnterWaitingState_Implementation()
{
    Super::OnEnterWaitingState_Implementation();

    TrySetFacilityState(EFacilityState::EFS_Running);
}

void AConveyorBelt::OnEnterRunningState_Implementation()
{
    Super::OnEnterRunningState_Implementation();
}

void AConveyorBelt::Tick_Running_Implementation(float Delta)
{
    Super::Tick_Running_Implementation(Delta);

    
    TryConnectToItemSender();
    TryReceiveItemFrom(GetItemSender().GetInterface());

    Tick_ItemAlongSpline(Delta);
    
    TryConnectItemReceiver();
    TrySendItemTo(GetItemReceiver().GetInterface());
}

void AConveyorBelt::InitializeSplineSpeed()
{
    SetSplineMoveSpeed(SplineComponent->GetSplineLength());
}