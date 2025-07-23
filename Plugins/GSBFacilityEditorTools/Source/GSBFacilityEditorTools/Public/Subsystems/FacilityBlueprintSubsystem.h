// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EditorSubsystem.h"
#include "FacilityBlueprintSubsystem.generated.h"

/**
 * 
 */
UCLASS()
class GSBFACILITYEDITORTOOLS_API UFacilityBlueprintSubsystem : public UEditorSubsystem
{
	GENERATED_BODY()
	
public:
	// void MakeLinkBeginConstructionToDissolveTimeLine(UBlueprint* Blueprint);

	void AddEventOverrideNode(UBlueprint* Blueprint, FName NativeEventName, bool bCallParent = true);

	UEdGraphNode* FindNodeByTitle(UEdGraph* EdGraph, const FString& Name);

	UEdGraph* FindEdGraphByName(const TArray<TObjectPtr<UEdGraph>>& EdGraphs, const FString& Name);

	UEdGraph* FindOrAddEventGraph(UBlueprint* Blueprint, FName NativeEventName, bool bCallParent = true);

	class UK2Node_Timeline* AddTimelineNode(UBlueprint* Blueprint, UEdGraph* EdGraph, const FName& TimelineName);

	UTimelineTemplate* FindOrCreateTimelineNode(UBlueprint* Blueprint, const FName& TimelineName);
};
