// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/Addon/ConfigurableOutputPort.h"
#include "Components/InteractionComponent.h"
#include "Components/ItemStorageComponent.h"
#include "Interfaces/HubFacility.h"
#include "HUDs/GSBOutputPortDetailWidget.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

AConfigurableOutputPort::AConfigurableOutputPort()
{
	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));
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
	if (DetailWidget.IsValid())
	{
		DetailWidget->SetSelectedItem(SelectedItem);
	}
}

void AConfigurableOutputPort::OnViewDetailInteraction(AActor* Interactor)
{
	if (DetailWidgetClass)
	{
		if (UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem< UGSBWindowSubsystem>())
		{
			DetailWidget = CreateWidget<UGSBOutputPortDetailWidget>(Interactor->GetInstigatorController<APlayerController>(), DetailWidgetClass);
			DetailWidget->SetLinkedOutputPort(this);
			DetailWidget->TryLinkItemStorageComponent(LinkedStorage.Get());
			DetailWidget->UpdateStorageWidget();
			UpdateSelectedItem(SelectedItem);
			WindowManager->OpenWindow(this, DetailWidget.Get());
		}
		else
		{
			TRACE_SCREEN_LOG(TEXT("DetailWidgetClass가 nullptr입니다."));
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("DetailWidgetClass가 nullptr입니다."));
	}
}
