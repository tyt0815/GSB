// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GhostPreviewComponent.h"
#include "GSBUtility.h"
#include "DebugHeader.h"

UGhostPreviewComponent::UGhostPreviewComponent()
{
	PrimaryComponentTick.bCanEverTick = false;

	SetVisibility(true, false);
}


void UGhostPreviewComponent::BeginPlay()
{
	Super::BeginPlay();
}

void UGhostPreviewComponent::BuildGhostPreview(AActor* PreviewActor)
{
	ClearGhostPreviewComponents();

	if (IsValid(PreviewActor))
	{
		CopyActorComponentsToPreview(PreviewActor, this);
	}
}

void UGhostPreviewComponent::ClearGhostPreviewComponents()
{
	for (USceneComponent* GhostPreviewComp : GhostPreviewComponents)
	{
		if (IsValid(GhostPreviewComp))
		{
			GhostPreviewComp->DestroyComponent();
		}
	}

	GhostPreviewComponents.Empty();
}


void UGhostPreviewComponent::CopyActorComponentsToPreview(AActor* SourceActor, USceneComponent* Target)
{
	if (IsValid(Target) && IsValid(SourceActor))
	{
		CopyComponentHierarchyToPreview(SourceActor->GetRootComponent(), Target);
	}
}

void UGhostPreviewComponent::CopyComponentHierarchyToPreview(USceneComponent* SourceComp, USceneComponent* Target)
{
	if (IsValid(Target) && IsValid(SourceComp))
	{
		FString NewChildCompName = TEXT("ChildComp") + FString::FromInt(GhostPreviewComponents.Num());
	
		USceneComponent* NewChild = nullptr;
		if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(SourceComp))
		{
			NewChildCompName += TEXT("_StaticMesh");
			UStaticMeshComponent* NewStaticMeshComp = NewObject<UStaticMeshComponent>(this, *NewChildCompName);
			NewStaticMeshComp->SetStaticMesh(StaticMeshComponent->GetStaticMesh());
			NewChild = NewStaticMeshComp;
		}

		if (!NewChild)
		{
			NewChild = NewObject<USceneComponent>(this, *NewChildCompName);
		}

		NewChild->SetupAttachment(Target);
		NewChild->RegisterComponent();

		NewChild->SetRelativeTransform(SourceComp->GetRelativeTransform());
		NewChild->SetVisibility(true);
		if (UPrimitiveComponent* NewPrimitive = Cast<UPrimitiveComponent>(NewChild))
		{
			UPrimitiveComponent* SourcePrimitive = Cast<UPrimitiveComponent>(SourceComp);
			NewPrimitive->SetCollisionProfileName(TEXT("PreviewMesh"));
			for (int i = 0; i < SourcePrimitive->GetNumMaterials(); ++i)
			{
				NewPrimitive->SetMaterial(i, SourcePrimitive->GetMaterial(i));
			}
		}

		GhostPreviewComponents.Add(NewChild);

		if (UChildActorComponent* ChildActorComponent = Cast<UChildActorComponent>(SourceComp))
		{
			CopyActorComponentsToPreview(ChildActorComponent->GetChildActor(), NewChild);
		}

		TArray<USceneComponent*> SourceCompChildren;
		SourceComp->GetChildrenComponents(false, SourceCompChildren);
		for (USceneComponent* CopyTargetChild : SourceCompChildren)
		{
			CopyComponentHierarchyToPreview(CopyTargetChild, NewChild);
		}
	}
}