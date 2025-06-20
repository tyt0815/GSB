// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/FacilityBuilder.h"
#include "Actors/ConstructibleFacility.h"
#include "Components/DecalComponent.h"
#include "Components/BoxComponent.h"
#include "GSBGameInstance.h"
#include "GSBDefines.h"
#include "DebugHeader.h"

AFacilityBuilder::AFacilityBuilder()
{
	PrimaryActorTick.bCanEverTick = true;
	SetTickGroup(ETickingGroup::TG_PostUpdateWork);
	SetRootComponent(CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent")));
	DecalComponent = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComponent"));
	DecalComponent->SetupAttachment(GetRootComponent());
	DecalComponent->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	DecalComponent->SetVisibility(false);
}

void AFacilityBuilder::Tick(float Delta)
{
	if (IsValid(FacilityPreview))
	{
		FacilityPreview->SnapActorToGrid(GetActorLocation());
		DecalComponent->SetWorldLocation(FacilityPreview->GetActorLocation());
		if (FacilityPreview->IsValidPlace())
		{
			DecalComponent->SetMaterial(0, ValidPlacementDecal);
		}
		else
		{
			DecalComponent->SetMaterial(0, InvalidPlacementDecal);
		}
	}
}

void AFacilityBuilder::PreviewFacility(TSubclassOf<AConstructibleFacility> FacilityClass)
{
	UWorld* World = GetWorld();
	if (World && FacilityClass)
	{
		if (IsValid(FacilityPreview))
		{
			CancelFacilityPreview();
		}
		FacilityPreview = World->SpawnActor<AConstructibleFacility>(FacilityClass);
		if (FacilityPreview)
		{
			check(FacilityPreview->TrySetFacilityState(EFacilityState::EFS_Preview));
			DecalComponent->SetVisibility(true);
			FVector FacilityPreviewBoxExtent = FacilityPreview->GetGridBoundsComponent()->GetScaledBoxExtent();
			DecalComponent->DecalSize = FVector(FacilityPreviewBoxExtent.Z + GRID_CELL_SIZE, FacilityPreviewBoxExtent.Y, FacilityPreviewBoxExtent.X);

			
			if (UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance()))
			{
				GameInst->SetPowerInfluenceVisibilityByFacility(FacilityPreview, true);
			}
		}
	}
}

void AFacilityBuilder::ConfirmFacilityPlacement()
{
	if (IsValid(FacilityPreview) && FacilityPreview->IsValidPlace())
	{
		if (FacilityPreview->TrySetFacilityState(EFacilityState::EFS_Constructing))
		{
			if (UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance()))
			{
				GameInst->SetPowerInfluenceVisibilityByFacility(FacilityPreview, false);
			}

			DecalComponent->SetVisibility(false);
			FacilityPreview = nullptr;
		}
	}
}

void AFacilityBuilder::CancelFacilityPreview()
{
	if (IsValid(FacilityPreview))
	{
		if (UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance()))
		{
			GameInst->SetPowerInfluenceVisibilityByFacility(FacilityPreview, false);
		}

		DecalComponent->SetVisibility(false);
		FacilityPreview->Destroy();
	}
	FacilityPreview = nullptr;
}

bool AFacilityBuilder::IsPlacingFacility()
{
	return FacilityPreview != nullptr;
}
