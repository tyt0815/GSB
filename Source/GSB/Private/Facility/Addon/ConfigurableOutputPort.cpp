// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/Addon/ConfigurableOutputPort.h"
#include "Components/InteractionComponent.h"
#include "Components/ItemStorageComponent.h"
#include "Components/MeshOverlayHelperComponent.h"
#include "Interfaces/HubFacility.h"
#include "HUDs/GSBOutputPortDetailWidget.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

AConfigurableOutputPort::AConfigurableOutputPort()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionRange"));
	InteractionComponent->SetupAttachment(GetRootComponent());

	MeshOverlayHelperComponent = CreateDefaultSubobject<UMeshOverlayHelperComponent>(TEXT("MeshOverlayHelper"));
}

void AConfigurableOutputPort::GetInteractionDescriptions(TArray<FString>& Descriptions)
{
	InteractionComponent->GetInteractionDescriptions(Descriptions);
}

void AConfigurableOutputPort::Interaction(int32 Index, AActor* Interactor)
{
	InteractionComponent->Interaction(Index, Interactor);
}

int32 AConfigurableOutputPort::GetNumInteractions() const
{
	return InteractionComponent->GetNumInteractions();
}

bool AConfigurableOutputPort::IsInteractable() const
{
	return InteractionComponent->IsInteractable();
}

void AConfigurableOutputPort::SetHighlighInteractableActor(bool bVisibility)
{
	MeshOverlayHelperComponent->SetOutlineVisibility(bVisibility);
	MeshOverlayHelperComponent->SetHighlightVisibility(bVisibility);
}

void AConfigurableOutputPort::BeginPlay()
{
	Super::BeginPlay();
	InteractionComponent->AddInteractionDynamic(TEXT("출력 포트"), this, &AConfigurableOutputPort::OnViewDetailInteraction);
	if (Owner->Implements<UHubFacility>())
	{
		LinkedStorage = Cast<IHubFacility>(Owner)->GetHubStorageComponent();
	}
}

void AConfigurableOutputPort::UpdateSelectedItem(UItemDataAsset* NewData)
{
	SelectedItem = NewData;
	DetailWindow->SetOutputItem(SelectedItem);
}

void AConfigurableOutputPort::OnViewDetailInteraction(AActor* Interactor)
{
	if (DetailWindowClass)
	{	
		UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
		
		DetailWindow = Cast<UGSBOutputPortDetailWindow>(WindowManager->OpenWindow(DetailWindowClass, TEXT("OutputPortDetailWindow")));
		DetailWindow->LinkOutputPort(this);
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("DetailWindowClass가 nullptr 입니다."));
	}
}
