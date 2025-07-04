// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SceneComponent.h"
#include "GhostPreviewComponent.generated.h"


UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSB_API UGhostPreviewComponent : public USceneComponent
{
	GENERATED_BODY()

public:	
	UGhostPreviewComponent();

protected:
	virtual void BeginPlay() override;
		
public:
	void BuildGhostPreview(AActor* PreviewActor);

	void ClearGhostPreviewComponents();

private:
	void CopyActorComponentsToPreview(AActor* SourceActor, USceneComponent* Target);

	void CopyComponentHierarchyToPreview(USceneComponent* SourceComp, USceneComponent* Target);

	UPROPERTY(VisibleAnywhere)
	TArray<TObjectPtr<USceneComponent>> GhostPreviewComponents;
};
