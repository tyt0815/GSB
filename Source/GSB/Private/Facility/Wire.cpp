// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/Wire.h"
#include "Components/SplineMeshComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

AWire::AWire()
{
	PrimaryActorTick.bCanEverTick = false;
	
	SplineMesh = CreateDefaultSubobject<USplineMeshComponent>(TEXT("SplineMesh"));
	SetRootComponent(SplineMesh);
	ConnectingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ConnectingTimeline"));
	ConnectingTimeline->SetLooping(false);
	ConnectingTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	DisconnectingTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DisconnectingTimeline"));
	DisconnectingTimeline->SetLooping(false);
	DisconnectingTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
}

void AWire::BeginPlay()
{
	Super::BeginPlay();

	TArray<UMaterialInterface*> SplineMeshMaterials = SplineMesh->GetMaterials();
	for (int i = 0; i < SplineMeshMaterials.Num(); ++i)
	{
		if (UMaterialInstanceDynamic* DynamicMatInst = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, SplineMeshMaterials[i]))
		{
			SplineMesh->SetMaterial(i, DynamicMatInst);
			SplineMeshDynamicMaterialInsts.Add(DynamicMatInst);
		}
	}
	InitializeLinkTimeline();
	InitializeUnlinkTimeline();
}

void AWire::Connect(const FVector& WorldStart, const FVector& WorldEnd)
{
	SetStartAndEndPosition(WorldStart, WorldEnd);

	DisconnectingTimeline->Stop();
	ConnectingTimeline->PlayFromStart();
}

void AWire::Disconnect()
{
	ConnectingTimeline->Stop();
	DisconnectingTimeline->PlayFromStart();
}

void AWire::SetStartAndEndPosition(const FVector& WorldStart, const FVector& WorldEnd)
{
	FVector LocalStart = ConvertWorldToLocalPosition(WorldStart);
	FVector LocalEnd = ConvertWorldToLocalPosition(WorldEnd);

	FVector Tangent = (LocalEnd - LocalStart).GetSafeNormal() * 100.f;

	SplineMesh->SetStartAndEnd(LocalStart, Tangent, LocalEnd, Tangent);
}

FVector AWire::ConvertWorldToLocalPosition(const FVector& WorldPosition)
{
	return GetRootComponent()->GetComponentTransform().InverseTransformPosition(WorldPosition);
}

void AWire::InitializeLinkTimeline()
{

	FOnTimelineFloatStatic OnTimelineFloatStatic;
	FOnTimelineEventStatic OnTimelineFinished;
	OnTimelineFloatStatic.BindUFunction(this, TEXT("OnConnecting"));
	OnTimelineFinished.BindUFunction(this, TEXT("EndConnecting"));

	if (UCurveFloat* LinearCurveFloat = UGSBGameInstance::GetCurveFloat(this, TEXT("LinearZeroToOne")))
	{
		ConnectingTimeline->SetTimelineLength(1);
		ConnectingTimeline->SetPlayRate(1.f / FMath::Max(0.00000001, DissolveTime));
		ConnectingTimeline->AddInterpFloat(LinearCurveFloat, OnTimelineFloatStatic);
		ConnectingTimeline->SetTimelineFinishedFunc(OnTimelineFinished);
	}
}

void AWire::InitializeUnlinkTimeline()
{
	FOnTimelineFloatStatic OnTimelineFloatStatic;
	OnTimelineFloatStatic.BindUFunction(this, TEXT("OnDisconnecting"));
	FOnTimelineEventStatic OnTimelineFinished;
	OnTimelineFinished.BindUFunction(this, TEXT("EndDisconnecting"));
	if (UCurveFloat* LinearCurveFloat = UGSBGameInstance::GetCurveFloat(this, TEXT("LinearZeroToOne")))
	{
		DisconnectingTimeline->SetTimelineLength(1);
		DisconnectingTimeline->SetPlayRate(1.f / FMath::Max(0.00000001, DissolveTime));
		DisconnectingTimeline->AddInterpFloat(LinearCurveFloat, OnTimelineFloatStatic);
		DisconnectingTimeline->SetTimelineFinishedFunc(OnTimelineFinished);
	}
}

void AWire::DissolveSplineMesh(float Amount)
{
	for (UMaterialInstanceDynamic* Material : SplineMeshDynamicMaterialInsts)
	{
		if (IsValid(Material))
		{
			Material->SetScalarParameterValue(TEXT("DissolveEffect_Amount"), Amount);
		}
	}
}

void AWire::OnConnecting(float Time)
{
	DissolveSplineMesh(LinkDissolveOffset + Time);
}

void AWire::EndConnecting()
{
	DissolveSplineMesh(1);
}

void AWire::OnDisconnecting(float Time)
{
	DissolveSplineMesh(1 - (Time + UnlinkDissolveOffset));
}

void AWire::EndDisconnecting()
{
	Destroy();
}
