#include "Facility/Facility.h"
#include "Facility/Addon/FacilityAddon.h"
#include "Interfaces/InteractableActor.h"
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

	InteractionComponent = CreateDefaultSubobject<UInteractionComponent>(TEXT("InteractionRange"));
	InteractionComponent->SetupAttachment(GetRootComponent());
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

UGSBWindowHead* AFacility::AttachDetailWindowHead(const TSubclassOf<UGSBWindowHead>& HeadClass)
{
	if (HeadClass)
	{
		if (UGSBWindowHead* Head = CreateWidget<UGSBWindowHead>(GetWorld()->GetFirstPlayerController(), HeadClass))
		{
			AttachDetailWindowHead(Head);
			return Head;
		}
		else
		{
			TRACE_SCREEN_LOG(TEXT("WindowHead생성 실패"));
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("WindowHeadClass가 유효하지 않습니다."));
	}
	return nullptr;
}

void AFacility::AttachDetailWindowHead(UGSBWindowHead* Head)
{
	if (IsValid(DetailWindow))
	{
		DetailWindow->AttachWindowHead(Head);
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("DetailWindow가 유효하지 않습니다."));
	}
}
