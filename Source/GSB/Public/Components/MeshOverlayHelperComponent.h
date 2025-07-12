// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "MeshOverlayHelperComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSB_API UMeshOverlayHelperComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	UMeshOverlayHelperComponent();

protected:
	virtual void BeginPlay() override;
		
public:
	UFUNCTION(BlueprintCallable, Category = "UMeshOverlayHelperComponent")
	void AddOutlineTarget(UMeshComponent* InOutlineTarget);

	UFUNCTION(BlueprintCallable, Category = "UMeshOverlayHelperComponent")
	void AddHighlightTarget(UMeshComponent* InHighlightTarget);

	UFUNCTION(BlueprintCallable, Category = "UMeshOverlayHelperComponent")
	void AddOutlineTargets(TArray<UMeshComponent*> InOutlineTargets);

	UFUNCTION(BlueprintCallable, Category = "UMeshOverlayHelperComponent")
	void AddHighlightTargets(TArray<UMeshComponent*> InHighlightTargets);

	UFUNCTION(BlueprintCallable, Category = "UMeshOverlayHelperComponent")
	void SetOutlineVisibility(bool bOutline);

	UFUNCTION(BlueprintCallable, Category = "UMeshOverlayHelperComponent")
	void SetHighlightVisibility(bool bHighlight);

	UFUNCTION(BlueprintCallable, Category = "UMeshOverlayHelperComponent")
	void DrawMeshOutline(UMeshComponent* MeshComponent);

	UFUNCTION(BlueprintCallable, Category = "UMeshOverlayHelperComponent")
	void HighlightMesh(UMeshComponent* MeshComponent);

	UFUNCTION(BlueprintCallable, Category = "UMeshOverlayHelperComponent")
	void RemoveOverlay(UMeshComponent* MeshComponent);

	void ClearOutlineTargetsArray();

	void ClearHighlightTargetsArray();

	void ClearOverlayTargetsArray();
private:
	TArray<UMeshComponent*> OutlineTargets;

	TArray<UMeshComponent*> HighlightTargets;

	UPROPERTY(EditAnywhere, Category = "UMeshOverlayHelperComponent")
	UMaterialInterface* OutlineOverlay;

	UPROPERTY(EditAnywhere, Category = "UMeshOverlayHelperComponent")
	UMaterialInterface* HighlightOverlay;
};
