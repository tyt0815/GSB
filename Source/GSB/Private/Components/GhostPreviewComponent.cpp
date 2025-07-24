// Fill out your copyright notice in the Description page of Project Settings.


#include "Components/GhostPreviewComponent.h"
#include "ProceduralMeshComponent.h"
#include "Components/InstancedStaticMeshComponent.h"
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
		if (UInstancedStaticMeshComponent* InstStaticMeshComp = Cast<UInstancedStaticMeshComponent>(SourceComp))
		{
			NewChildCompName += TEXT("_InstancedStaticMeshComponent");
			UInstancedStaticMeshComponent* NewInstStaticMeshComp = NewObject<UInstancedStaticMeshComponent>(this, *NewChildCompName);
			NewInstStaticMeshComp->SetStaticMesh(InstStaticMeshComp->GetStaticMesh());
			for (int i = 0; i < InstStaticMeshComp->GetNumInstances(); ++i)
			{
				FTransform Transform = {};
				InstStaticMeshComp->GetInstanceTransform(i, Transform);
				NewInstStaticMeshComp->AddInstance(Transform);
			}
			NewInstStaticMeshComp->MarkRenderStateDirty();
			NewChild = NewInstStaticMeshComp;
		}
		else if (UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(SourceComp))
		{
			NewChildCompName += TEXT("_StaticMesh");
			UStaticMeshComponent* NewStaticMeshComp = NewObject<UStaticMeshComponent>(this, *NewChildCompName);
			NewStaticMeshComp->SetStaticMesh(StaticMeshComponent->GetStaticMesh());
			NewChild = NewStaticMeshComp;
		}
		else if (UProceduralMeshComponent* ProceduralMeshComponent = Cast<UProceduralMeshComponent>(SourceComp))
		{
			NewChildCompName += TEXT("_ProceduralMesh");
			UProceduralMeshComponent* NewProcMeshComp = NewObject<UProceduralMeshComponent>(this, *NewChildCompName);
			int i = 0;
			while (FProcMeshSection* ProcMeshSection = ProceduralMeshComponent->GetProcMeshSection(i++))
			{
				TArray<FVector> Vertices;
				TArray<FVector> Normals;
				TArray<FVector2D> UV0;
				TArray<FVector2D> UV1;
				TArray<FVector2D> UV2;
				TArray<FVector2D> UV3;
				TArray<FColor> VertexColors;
				TArray<FProcMeshTangent> Tangents;

				for (const FProcMeshVertex& ProcMeshVertex : ProcMeshSection->ProcVertexBuffer)
				{
					Vertices.Add(ProcMeshVertex.Position);
					Normals.Add(ProcMeshVertex.Normal);
					UV0.Add(ProcMeshVertex.UV0);
					UV1.Add(ProcMeshVertex.UV1);
					UV2.Add(ProcMeshVertex.UV2);
					UV3.Add(ProcMeshVertex.UV3);
					VertexColors.Add(ProcMeshVertex.Color);
					Tangents.Add(ProcMeshVertex.Tangent);
				}
				TArray<int32> Indices;
				for (const uint32& Index : ProcMeshSection->ProcIndexBuffer)
				{
					Indices.Add(Index);
				}

				NewProcMeshComp->CreateMeshSection(i, Vertices, Indices, Normals, UV0, UV1, UV2, UV3, VertexColors, Tangents, false);
			}

			NewChild = NewProcMeshComp;
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