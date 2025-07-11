#include "Facility/Facility.h"
#include "Facility/Addon/FacilityAddon.h"
#include "Interfaces/InteractableActor.h"
#include "Components/InteractionComponent.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "HUDs/GSBWindowWidget.h"
#include "HUDs/GSBWindowBody.h"
#include "HUDs/GSBWindowHead.h"
#include "GSBDefines.h"
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
	if (DetailWindowBodyClass)
	{
		DetailWindowBody = CreateWidget<UGSBWindowBody>(Interactor->GetInstigatorController<APlayerController>(), DetailWindowBodyClass);
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("DetailWindowBodyClass가 nullptr입니다."))
	}

	UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
	DetailWindow = WindowManager->OpenWindow(this, DetailWindowBody);
	if (IsValid(DetailWindow))
	{
		DetailWindow->ClearWindowHead();
	}
}

void AFacility::AttachDetailWindowHead(UGSBWindowHead* Head)
{
	if (IsValid(DetailWindow))
	{
		DetailWindow->AttachWindowHead(Head);
	}
}
