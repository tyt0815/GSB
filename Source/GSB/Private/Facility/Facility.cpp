#include "Facility/Facility.h"

#include "GSBDefines.h"
#include "Interfaces/InteractableActor.h"
#include "Components/InteractionComponent.h"
#include "Facility/Addon/FacilityAddon.h"
#include "HUDs/GSBFacilityDetailWidget.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

AFacility::AFacility()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetCollisionProfileName(TEXT("FacilityMesh"));

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("Interaction"));
}

void AFacility::GetInteractionDescriptions(TArray<FString>& Descriptions)
{
	InteractionComponent->GetInteractionDescriptions(Descriptions);
}

void AFacility::Interaction(int32 Index, AActor* Interactor)
{
	InteractionComponent->Interaction(Index, Interactor);
}

int32 AFacility::GetNumInteractions() const
{
	return InteractionComponent->GetNumInteractions();
}

void AFacility::BeginPlay()
{
	Super::BeginPlay();

	InteractionComponent->AddInteractionDynamic(FacilityName.ToString(), this, &AFacility::OnShowDetailInteraction);
}

void AFacility::ConnectFacilityAddon(AFacilityAddon* Addon)
{
	ConnectedAddons.AddUnique(Addon);
	Addon->OnConnectedToFacility();
}

void AFacility::OnShowDetailInteraction(AActor* Interactor)
{
	if (DetailWidgetClass)
	{
		UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
		DetailWidget = CreateWidget<UGSBFacilityDetailWidget>(Interactor->GetInstigatorController<APlayerController>(), DetailWidgetClass);
		WindowManager->OpenWindow(DetailWidget);
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("DetailWidgetClass가 nullptr입니다."))
	}
}
