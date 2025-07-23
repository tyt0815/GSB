// Fill out your copyright notice in the Description page of Project Settings.


#include "Subsystems/FacilityBlueprintSubsystem.h"
#include "Kismet2/BlueprintEditorUtils.h"
#include "Kismet2/KismetEditorUtilities.h"
#include "K2Node_Event.h"
#include "K2Node_CallParentFunction.h"
#include "K2Node_FunctionEntry.h"
#include "K2Node_Timeline.h"
#include "Engine/TimelineTemplate.h"
#include "GSBDebugLibrary/Public/GSBDebugLibrary.h"

//void UFacilityBlueprintSubsystem::MakeLinkBeginConstructionToDissolveTimeLine(UBlueprint* Blueprint)
//{
//	if (!IsValid(Blueprint))
//	{
//		TRACE_PRINT_SCREEN_AND_LOG(TEXT("Invalid Blueprint"));
//		return;
//	}
//
//	UEdGraph* BeginConstructionEventGraph = FindOrAddEventGraph(Blueprint, TEXT("BeginConstruction"));
//	if (!BeginConstructionEventGraph)
//	{
//		TRACE_PRINT_SCREEN_AND_LOG(TEXT("Invalid graph"));
//		return;
//	}
//
//	UK2Node_FunctionEntry* BeginConstructionNode = Cast<UK2Node_FunctionEntry>(FindNodeByTitle(BeginConstructionEventGraph, TEXT("BeginConstruction")));
//	if (!BeginConstructionNode)
//	{
//		TRACE_PRINT_SCREEN_AND_LOG(TEXT("Invalid node"));
//		return;
//	}
//
//	UEdGraphPin* StartPin = BeginConstructionNode->FindPin(TEXT("then"));
//	TConstArrayView<UEdGraphPin*> EndPins = StartPin->LinkedTo;
//
//
//	// AddTimelineNode(Blueprint, Blueprint->UbergraphPages[0], TEXT("sibal"));
//	UTimelineTemplate* TimelineTemplate = FindOrCreateTimelineNode(Blueprint, TEXT("sibal"));
//	if (UK2Node_Timeline* TimelineNode = FBlueprintEditorUtils::FindNodeForTimeline(Blueprint, TimelineTemplate))
//	{
//		Blueprint->UbergraphPages[0]->AddNode(TimelineNode);
//	}
//	else
//	{
//		TRACE_PRINT_SCREEN_AND_LOG(TEXT("sibal"));
//	}
//
//}

void UFacilityBlueprintSubsystem::AddEventOverrideNode(UBlueprint* Blueprint, FName NativeEventName, bool bCallParent)
{
	if (!Blueprint || Blueprint->UbergraphPages.Num() == 0)
	{
		TRACE_PRINT_SCREEN_AND_LOG(TEXT("Invalid Blueprint or no event graph."));
		return;
	}

	// 이벤트 시그니처 찾기
	UFunction* Function = Blueprint->ParentClass->FindFunctionByName(NativeEventName);
	if (!Function || !Function->HasAnyFunctionFlags(FUNC_BlueprintEvent))
	{
		TRACE_PRINT_SCREEN_AND_LOG(FString::Printf(TEXT("Function %s is not a BlueprintNativeEvent."), *NativeEventName.ToString()));
		return;
	}

	UEdGraph* EventGraph = Blueprint->UbergraphPages[0];

	// 이벤트 노드 생성
	UK2Node_Event* EventNode = NewObject<UK2Node_Event>(EventGraph);
	check(EventNode);

	// 함수 참조 구성 (중요!)
	EventNode->EventReference.SetFromField<UFunction>(Function, Blueprint->ParentClass);
	EventNode->bOverrideFunction = true;

	// 위치 및 초기화
	EventNode->NodePosX = 200;
	EventNode->NodePosY = 100;
	EventNode->CreateNewGuid();
	EventNode->AllocateDefaultPins();
	EventNode->PostPlacedNewNode();
	EventGraph->AddNode(EventNode);

	// Parent 이벤트 노드 생성
	if (bCallParent)
	{
		UK2Node_CallParentFunction* ParentNode = NewObject<UK2Node_CallParentFunction>(EventGraph);
		check(ParentNode);
		ParentNode->SetFromFunction(Function);
		ParentNode->NodePosX = 500;
		ParentNode->NodePosY = 200;
		ParentNode->CreateNewGuid();
		ParentNode->AllocateDefaultPins();
		ParentNode->PostPlacedNewNode();
		EventGraph->AddNode(ParentNode);

		EventNode->FindPin(TEXT("then"))->MakeLinkTo(ParentNode->FindPin(TEXT("execute")));
	}
	
	// 컴파일
	FKismetEditorUtilities::CompileBlueprint(Blueprint);
}

UEdGraphNode* UFacilityBlueprintSubsystem::FindNodeByTitle(UEdGraph* EdGraph, const FString& Name)
{
	for (UEdGraphNode* Node : EdGraph->Nodes)
	{
		if (Node->GetNodeTitle(ENodeTitleType::FullTitle).ToString() == Name)
		{
			return Node;
		}
	}
	
	return nullptr;
}

UEdGraph* UFacilityBlueprintSubsystem::FindEdGraphByName(const TArray<TObjectPtr<UEdGraph>>& EdGraphs, const FString& Name)
{
	for (const TObjectPtr<UEdGraph>& Graph : EdGraphs)
	{
		if (IsValid(Graph) && Graph->GetName() == Name)
		{
			return Graph.Get();
		}
	}
	return nullptr;
}

UEdGraph* UFacilityBlueprintSubsystem::FindOrAddEventGraph(UBlueprint* Blueprint, FName NativeEventName, bool bCallParent)
{
	const TArray<TObjectPtr<UEdGraph>>& EventGraphs = Blueprint->EventGraphs;
	UEdGraph* BeginConstructionEventGraph = FindEdGraphByName(EventGraphs, NativeEventName.ToString());

	if (!BeginConstructionEventGraph)
	{
		AddEventOverrideNode(Blueprint, NativeEventName);
		BeginConstructionEventGraph = FindEdGraphByName(EventGraphs, NativeEventName.ToString());
	}
	return BeginConstructionEventGraph;
}

UK2Node_Timeline* UFacilityBlueprintSubsystem::AddTimelineNode(UBlueprint* Blueprint, UEdGraph* EdGraph, const FName& TimelineName)
{
	if (IsValid(Blueprint) && IsValid(EdGraph))
	{
		int32 Index = FBlueprintEditorUtils::FindTimelineIndex(Blueprint, TimelineName);
		if (Index >= 0)
		{
			TRACE_PRINT_SCREEN(TEXT("sex"));
			return FBlueprintEditorUtils::FindNodeForTimeline(Blueprint, Blueprint->Timelines[Index]);
		}
		else if (UK2Node_Timeline* TimelineNode = NewObject<UK2Node_Timeline>(EdGraph))
		{
			TimelineNode->CreateNewGuid();              // GUID 부여
			TimelineNode->PostPlacedNewNode();          // 새 노드 초기화
			TimelineNode->AllocateDefaultPins();        // 핀 생성
			EdGraph->AddNode(TimelineNode, true, false);

			FKismetEditorUtilities::CompileBlueprint(Blueprint);
			return TimelineNode;
		}
	}	
	return nullptr;
}

UTimelineTemplate* UFacilityBlueprintSubsystem::FindOrCreateTimelineNode(UBlueprint* Blueprint, const FName& TimelineName)
{
	if (IsValid(Blueprint))
	{
		int32 Index = FBlueprintEditorUtils::FindTimelineIndex(Blueprint, TimelineName);
		if (Index >= 0)
		{
			TRACE_PRINT_SCREEN(TEXT("sex"));
			return Blueprint->Timelines[Index].Get();
		}
		else if(UTimelineTemplate* TimelineTemplate = FBlueprintEditorUtils::AddNewTimeline(Blueprint, TimelineName))
		{
			FKismetEditorUtilities::CompileBlueprint(Blueprint); 
			return TimelineTemplate;
		}
	}
	return nullptr;
}
