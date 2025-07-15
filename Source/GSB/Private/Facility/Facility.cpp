#include "Facility/Facility.h"
#include "Facility/Addon/FacilityAddon.h"
#include "Interfaces/InteractableActor.h"
#include "Components/MeshOverlayHelperComponent.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "GSBGameInstance.h"
#include "GSBDefines.h"
#include "DebugHeader.h"

AFacility::AFacility()
{
	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("StaticMesh"));
	StaticMeshComponent->SetupAttachment(GetRootComponent());
	StaticMeshComponent->SetCollisionProfileName(TEXT("FacilityMesh"));

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionRange"));
	InteractionComponent->SetupAttachment(GetRootComponent());

	MeshOverlayHelperComponent = CreateDefaultSubobject<UMeshOverlayHelperComponent>(TEXT("MeshOverlayHelper"));
}

void AFacility::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (IsOperating())
	{
		Tick_OnOperating(DeltaSeconds);
	}
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

bool AFacility::IsInteractable() const
{
	return InteractionComponent->IsInteractable();
}

void AFacility::SetHighlighInteractableActor(bool bVisibility)
{
	MeshOverlayHelperComponent->SetOutlineVisibility(bVisibility);
	MeshOverlayHelperComponent->SetHighlightVisibility(bVisibility);
}

void AFacility::BeginPlay()
{
	Super::BeginPlay();

	InteractionComponent->AddInteractionDynamic(FacilityName.ToString(), this, &AFacility::OnShowDetailInteraction);

	MeshOverlayHelperComponent->AddHighlightTarget(StaticMeshComponent);
}

void AFacility::ConnectFacilityAddon(AFacilityAddon* Addon)
{
	ConnectedAddons.AddUnique(Addon);
	Addon->OnConnectedToFacility();
}

void AFacility::OnShowDetailInteraction(AActor* Interactor)
{
	if (DetailWindowClass)
	{
		if (UGSBGameInstance* GameInst = GetGameInstance<UGSBGameInstance>())
		{
			if (UGSBWindowSubsystem* WindowManager = GameInst->GetSubsystem<UGSBWindowSubsystem>())
			{
				DetailWindow = Cast<UGSBFacilityDetailWindow>(WindowManager->OpenWindow(DetailWindowClass, FName(FacilityName.ToString() + TEXT(" Detail Window"))));
				if (DetailWindow)
				{
					DetailWindow->OnLinkedToFacility(this);
				}
			}
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("DetailWindowClass가 nullptr입니다. :") + FacilityName.ToString());
	}
}

void AFacility::UnlinkFacilityDetailWindow()
{
	if (DetailWindow)
	{
		DetailWindow->OnUnlinkedFromFacility();
	}
	DetailWindow = nullptr;
}
