// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/FacilityGhostActor.h"
#include "Facility/Facility.h"
#include "Components/GhostPreviewComponent.h"
#include "Components/DecalComponent.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

AFacilityGhostActor::AFacilityGhostActor()
{
	PrimaryActorTick.bCanEverTick = false;

	GhostPreviewComponent = CreateDefaultSubobject<UGhostPreviewComponent>(TEXT("GhostPreivew"));
	GhostPreviewComponent->SetupAttachment(GetRootComponent());
	PlacementDecal = CreateDefaultSubobject<UDecalComponent>(TEXT("PlacementDecal"));
	PlacementDecal->SetupAttachment(GetRootComponent());
	PlacementDecal->SetRelativeRotation(FRotator(90, 0, 0));
}

void AFacilityGhostActor::BuildGhostPreview(AFacility* Facility)
{
	GhostPreviewComponent->BuildGhostPreview(Facility);
	ResizeGridBounds(Facility->GetGridBoundsSize());

	FVector GridBoundsExtent = GetGridBoxExtent();
	PlacementDecal->DecalSize = FVector(GridBoundsExtent.Z + 50, GridBoundsExtent.X, GridBoundsExtent.Y);
}

void AFacilityGhostActor::UpdatePlacementDecal(bool bValid)
{
	if (UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance()))
	{
		if (bValid)
		{
			if (UMaterialInterface* ValidDecal = GameInst->GetMaterialInterface(TEXT("VaildPlacementDecal")))
			{
				PlacementDecal->SetMaterial(0, ValidDecal);
			}
			else
			{
				TRACE_SCREEN_LOG(TEXT("VaildPlacementDecal가 nullptr 입니다."));
			}
		}
		else
		{
			if (UMaterialInterface* InvalidDecal = GameInst->GetMaterialInterface(TEXT("InvaildPlacementDecal")))
			{
				PlacementDecal->SetMaterial(0, InvalidDecal);
			}
			else
			{
				TRACE_SCREEN_LOG(TEXT("InvaildPlacementDecal가 nullptr 입니다."));
			}
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("UGSBGameInstance 캐스팅 실패"));
	}
}
