// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/MeshOverlayHelperComponent.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

UMeshOverlayHelperComponent::UMeshOverlayHelperComponent()
{
	PrimaryComponentTick.bCanEverTick = false;
}

void UMeshOverlayHelperComponent::BeginPlay()
{
	Super::BeginPlay();

	if (UGSBGameInstance* GameInst = GetWorld()->GetGameInstance<UGSBGameInstance>())
	{
		if (OutlineOverlay == nullptr)
		{
			OutlineOverlay = GameInst->GetMaterialInterface("OutlineOverlay");
		}
		if (HighlightOverlay == nullptr)
		{
			HighlightOverlay = GameInst->GetMaterialInterface("HighlightOverlay");
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("UGSBGameInstance 캐스팅 실패"));
	}
}

void UMeshOverlayHelperComponent::AddOutlineTarget(UMeshComponent* InOutlineTarget)
{
	OutlineTargets.AddUnique(InOutlineTarget);
}

void UMeshOverlayHelperComponent::AddHighlightTarget(UMeshComponent* InHighlightTarget)
{
	HighlightTargets.AddUnique(InHighlightTarget);
}

void UMeshOverlayHelperComponent::AddOutlineTargets(TArray<UMeshComponent*> InOutlineTargets)
{
	for (UMeshComponent* MeshComp : InOutlineTargets)
	{
		AddOutlineTarget(MeshComp);
	}
}

void UMeshOverlayHelperComponent::AddHighlightTargets(TArray<UMeshComponent*> InHighlightTargets)
{
	for (UMeshComponent* MeshComp : InHighlightTargets)
	{
		AddHighlightTarget(MeshComp);
	}
}

void UMeshOverlayHelperComponent::SetOutlineVisibility(bool bOutline)
{
	ClearOutlineTargetsArray();
	if (bOutline)
	{
		for (UMeshComponent* MeshComp : OutlineTargets)
		{
			DrawMeshOutline(MeshComp);
		}
	}
	else
	{
		for (UMeshComponent* MeshComp : OutlineTargets)
		{
			RemoveOverlay(MeshComp);
		}
	}
}

void UMeshOverlayHelperComponent::SetHighlightVisibility(bool bHighlight)
{
	ClearHighlightTargetsArray();
	if (bHighlight)
	{
		for (UMeshComponent* MeshComp : HighlightTargets)
		{
			HighlightMesh(MeshComp);
		}
	}
	else
	{
		for (UMeshComponent* MeshComp : HighlightTargets)
		{
			RemoveOverlay(MeshComp);
		}
	}
}

void UMeshOverlayHelperComponent::DrawMeshOutline(UMeshComponent* MeshComponent)
{
	MeshComponent->SetOverlayMaterial(OutlineOverlay);
}

void UMeshOverlayHelperComponent::HighlightMesh(UMeshComponent* MeshComponent)
{
	MeshComponent->SetOverlayMaterial(HighlightOverlay);
}

void UMeshOverlayHelperComponent::RemoveOverlay(UMeshComponent* MeshComponent)
{
	MeshComponent->SetOverlayMaterial(nullptr);
}

void UMeshOverlayHelperComponent::ClearOutlineTargetsArray()
{
	for (int i = OutlineTargets.Num() - 1; i >= 0; --i)
	{
		if (!IsValid(OutlineTargets[i]))
		{
			OutlineTargets.RemoveAt(i);
		}
	}
}

void UMeshOverlayHelperComponent::ClearHighlightTargetsArray()
{
	for (int i = HighlightTargets.Num() - 1; i >= 0; --i)
	{
		if (!IsValid(HighlightTargets[i]))
		{
			HighlightTargets.RemoveAt(i);
		}
	}
}

void UMeshOverlayHelperComponent::ClearOverlayTargetsArray()
{
	ClearOutlineTargetsArray();
	ClearHighlightTargetsArray();
}