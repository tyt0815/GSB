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
	LinkTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("LinkTimeline"));
	LinkTimeline->SetLooping(false);
	LinkTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	UnlinkTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("UnlinkTimeline"));
	UnlinkTimeline->SetLooping(false);
	UnlinkTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
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

void AWire::Link(const FVector& WorldStart, const FVector& WorldEnd)
{
	SetStartAndEndPosition(WorldStart, WorldEnd);

	UnlinkTimeline->Stop();
	LinkTimeline->PlayFromStart();
}

void AWire::Unlink()
{
	LinkTimeline->Stop();
	UnlinkTimeline->PlayFromStart();
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
	OnTimelineFloatStatic.BindLambda(
		[this](float Time)
		{
			DissolveSplineMesh(LinkDissolveOffset + Time);
		}
	);
	FOnTimelineEventStatic OnTimelineFinished;
	OnTimelineFinished.BindLambda(
		[this]()
		{
			DissolveSplineMesh(1);
		}
	);

	if (UCurveFloat* LinearCurveFloat = UGSBGameInstance::GetCurveFloat(this, TEXT("LinearZeroToOne")))
	{
		LinkTimeline->SetTimelineLength(1);
		LinkTimeline->SetPlayRate(1.f / FMath::Max(0.00000001, DissolveTime));
		LinkTimeline->AddInterpFloat(LinearCurveFloat, OnTimelineFloatStatic);
		LinkTimeline->SetTimelineFinishedFunc(OnTimelineFinished);
	}
}

void AWire::InitializeUnlinkTimeline()
{
	FOnTimelineFloatStatic OnTimelineFloatStatic;
	OnTimelineFloatStatic.BindLambda(
		[this](float Time)
		{
			DissolveSplineMesh(1 - (Time + UnlinkDissolveOffset));
		}
	);
	FOnTimelineEventStatic OnTimelineFinished;
	OnTimelineFinished.BindLambda(
		[this]()
		{
			Destroy();
		}
	);

	if (UCurveFloat* LinearCurveFloat = UGSBGameInstance::GetCurveFloat(this, TEXT("LinearZeroToOne")))
	{
		UnlinkTimeline->SetTimelineLength(1);
		UnlinkTimeline->SetPlayRate(1.f / FMath::Max(0.00000001, DissolveTime));
		UnlinkTimeline->AddInterpFloat(LinearCurveFloat, OnTimelineFloatStatic);
		UnlinkTimeline->SetTimelineFinishedFunc(OnTimelineFinished);
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
