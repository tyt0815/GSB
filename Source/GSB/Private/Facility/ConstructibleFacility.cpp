// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ConstructibleFacility.h"
#include "Facility/Addon/FacilityAddon.h"
#include "Interfaces/PowerProviderFacility.h"
#include "Components/ItemStorageComponent.h"
#include "Components/TimelineComponent.h"
#include "Kismet/KismetMaterialLibrary.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

AConstructibleFacility::AConstructibleFacility()
{
	ConstructionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("ConstructionTimeline"));
	ConstructionTimeline->SetLooping(false);
	ConstructionTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
	DeconstructionTimeline = CreateDefaultSubobject<UTimelineComponent>(TEXT("DeconstructionTimeline"));
	DeconstructionTimeline->SetLooping(false);
	DeconstructionTimeline->SetTimelineLengthMode(ETimelineLengthMode::TL_TimelineLength);
}

bool AConstructibleFacility::IsOperating() const
{
	return Super::IsOperating() && IsConstructed();
}

void AConstructibleFacility::AddDefaultInteractions()
{
	Super::AddDefaultInteractions();
	InteractionComponent->AddInteractionDynamic(TEXT("철거"), this, &AConstructibleFacility::HandleDeconstructRequest);
}

void AConstructibleFacility::BeginPlay()
{
	Super::BeginPlay();

	InitializeConstructionTimeline();
	InitializeDeconstructionTimeline();

	CreateAllDynamicMaterialInstances();

	CompleteConstruction();
}

bool AConstructibleFacility::IsConstructed() const
{
	return !IsConstructing() && !IsDeconstructing();
}

bool AConstructibleFacility::IsConstructing() const
{
	return ConstructionTimeline->IsPlaying();
}

bool AConstructibleFacility::IsDeconstructing() const
{
	return DeconstructionTimeline->IsReversing();
}

float AConstructibleFacility::GetConstructionProgress() const
{
	return ConstructionTimeline->GetPlaybackPosition() / FMath::Max(ConstructionTimeline->GetTimelineLength(), 0.000000001);
}

float AConstructibleFacility::GetDeconstructionProgress() const
{
	return DeconstructionTimeline->GetPlaybackPosition() / FMath::Max(DeconstructionTimeline->GetTimelineLength(), 0.000000001);
}

bool AConstructibleFacility::TryBeginConstruction()
{
	if (IsConstructing())
	{
		return false;
	}
	BeginConstruction();
	return true;
}

bool AConstructibleFacility::TryBeginDeconstruction()
{
	if (IsConstructed())
	{
		BeginDeconstruction();
		return true;
	}
	return false;
}

void AConstructibleFacility::CreateAllDynamicMaterialInstances()
{
	TArray<UStaticMeshComponent*> AllStaticMeshComponents;
	GetComponents<UStaticMeshComponent>(AllStaticMeshComponents, true);
	for (UStaticMeshComponent* StaticMeshComp : AllStaticMeshComponents)
	{
		if (!IsValid(StaticMeshComp))
		{
			continue;
		}
		TArray<UMaterialInterface*> AllMaterials = StaticMeshComp->GetMaterials();
		for (int i = 0; i < AllMaterials.Num(); ++i)
		{
			if (!IsValid(AllMaterials[i]))
			{
				continue;
			}
			if (UMaterialInstanceDynamic* DynamicMaterialInstances = UKismetMaterialLibrary::CreateDynamicMaterialInstance(this, AllMaterials[i]))
			{
				StaticMeshComp->SetMaterial(i, DynamicMaterialInstances);
				AllDynamicMaterialInstances.Add(DynamicMaterialInstances);
			}
		}
	}
}

void AConstructibleFacility::BeginConstruction_Implementation()
{
	InteractionComponent->ClearInteractions();

	InteractionComponent->AddInteractionDynamic(TEXT("건설 취소"), this, &AConstructibleFacility::HandleCancelConstruction);

	if (ConstructionTime == 0)
	{
		CompleteConstruction();
	}
	else
	{
		ConstructionTimeline->PlayFromStart();
	}
}
void AConstructibleFacility::HandleCancelConstruction(AActor* Interactor)
{
	Destroy();
}
void AConstructibleFacility::HandleDeconstructRequest(AActor* Interactor)
{
	if (IsConstructing())
	{
		CompleteConstruction();
	}
	else if(IsConstructed())
	{
		TryBeginDeconstruction();
	}
}
void AConstructibleFacility::BeginDeconstruction_Implementation()
{
	if (IsDeconstructing())
	{
		return;	
	}
	if (IsValid(DetailWindow))
	{
		DetailWindow->Close();
	}

	InteractionComponent->DeactivateInteraction();
	SetHighlighInteractableActor(false);
	if (DeconstructionTime == 0)
	{
		CompleteDeconstruction();
	}
	else
	{
		DeconstructionTimeline->PlayFromStart();
	}
}

void AConstructibleFacility::OnConstructing_Implementation(float Progress)
{
	SetDissolveEffect(Progress);
}

void AConstructibleFacility::OnDeconstructing_Implementation(float Progress)
{
	SetDissolveEffect(1 - Progress);
}
void AConstructibleFacility::CompleteConstruction_Implementation()
{
	InteractionComponent->ClearInteractions();
	AddDefaultInteractions();

	SetDissolveEffect(1);

	for (AFacilityAddon* Addon : ConnectedAddons)
	{
		Addon->CompleteConstruction();
	}
}

void AConstructibleFacility::CompleteDeconstruction_Implementation()
{
	TArray<UItemStorageComponent*> ItemStorageComponents;
	GetComponents(ItemStorageComponents);
	for (UItemStorageComponent* ItemStorage : ItemStorageComponents)
	{
		ItemStorage->DropAllItems();
	}

	SetDissolveEffect(0);

	Destroy();
}

void AConstructibleFacility::InitializeConstructionTimeline()
{
	FOnTimelineFloatStatic OnTimelineFloatStatic;
	OnTimelineFloatStatic.BindLambda(
		[this](float Time)
		{
			OnConstructing(GetConstructionProgress());
		}
	);
	FOnTimelineEventStatic OnTimelineFinished;
	OnTimelineFinished.BindLambda(
		[this]()
		{
			CompleteConstruction();
		}
	);
	if (UGSBGameInstance* GameInst = GetGameInstance<UGSBGameInstance>())
	{
		UCurveFloat* LinearCurveFloat = GameInst->GetCurveFloat(TEXT("LinearZeroToOne"));
		ConstructionTimeline->SetTimelineLength(1);
		ConstructionTimeline->SetPlayRate(1.f / FMath::Max(0.00000001, ConstructionTime));
		ConstructionTimeline->AddInterpFloat(LinearCurveFloat, OnTimelineFloatStatic);
		ConstructionTimeline->SetTimelineFinishedFunc(OnTimelineFinished);
	}
}

void AConstructibleFacility::InitializeDeconstructionTimeline()
{
	FOnTimelineFloatStatic OnTimelineFloatStatic;
	OnTimelineFloatStatic.BindLambda(
		[this](float Time)
		{
			OnDeconstructing(GetDeconstructionProgress());
		}
	);
	FOnTimelineEventStatic OnTimelineFinished;
	OnTimelineFinished.BindLambda(
		[this]()
		{
			CompleteDeconstruction();
		}
	);
	if (UGSBGameInstance* GameInst = GetGameInstance<UGSBGameInstance>())
	{
		UCurveFloat* LinearCurveFloat = GameInst->GetCurveFloat(TEXT("LinearZeroToOne"));
		DeconstructionTimeline->SetTimelineLength(1);
		DeconstructionTimeline->SetPlayRate(1.f / FMath::Max(0.00000001, DeconstructionTime));
		DeconstructionTimeline->AddInterpFloat(LinearCurveFloat, OnTimelineFloatStatic);
		DeconstructionTimeline->SetTimelineFinishedFunc(OnTimelineFinished);
	}
}

void AConstructibleFacility::SetDissolveEffect(float Amount)
{
	for (UMaterialInstanceDynamic* DynamicMaterialInstances : AllDynamicMaterialInstances)
	{
		DynamicMaterialInstances->SetScalarParameterValue(TEXT("DissolveEffect_Amount"), Amount);
	}
}