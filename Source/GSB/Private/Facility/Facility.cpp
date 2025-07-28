#include "Facility/Facility.h"
#include "Facility/Addon/FacilityAddon.h"
#include "Facility/GSBFacilityDataAsset.h"
#include "Interfaces/InteractableActor.h"
#include "Components/MeshOverlayHelperComponent.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GSBGameInstance.h"
#include "GSBDefines.h"
#include "DebugHeader.h"

AFacility::AFacility()
{
	GetRootComponent()->Mobility = EComponentMobility::Static;
	MeshComponent = CreateDefaultSubobject<UChildActorComponent>(TEXT("Mesh"));
	MeshComponent->SetupAttachment(GetRootComponent());
	MeshComponent->Mobility = EComponentMobility::Static;

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

void AFacility::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	if (IsValid(DetailWindow))
	{
		DetailWindow->Close();
	}

	Super::EndPlay(EndPlayReason);
}

void AFacility::GetInteractionDescriptions(TArray<FString>& Descriptions)
{
	InteractionComponent->GetInteractionDescriptions(Descriptions);
}

void AFacility::Interaction(int32 Index, AActor* Interactor)
{
	InteractionComponent->Interaction(Index, Interactor);
}

void AFacility::InteractionByDescription(const FString& Description, AActor* Interactor)
{
	InteractionComponent->InteractionByDescription(Description, Interactor);
}

int32 AFacility::GetNumInteractions() const
{
	return InteractionComponent->GetNumInteractions();
}

bool AFacility::IsInteractable() const
{
	return InteractionComponent->IsInteractable();
}

bool AFacility::IsInteractionListDirty() const
{
	return InteractionComponent->IsInteractionListDirty();
}

void AFacility::ClearInteractionListDirtyFlag()
{
	InteractionComponent->ClearInteractionListDirtyFlag();
}

void AFacility::SetHighlighInteractableActor(bool bVisibility)
{
	MeshOverlayHelperComponent->SetOutlineVisibility(bVisibility);
	MeshOverlayHelperComponent->SetHighlightVisibility(bVisibility);
}

void AFacility::BeginPlay()
{
	Super::BeginPlay();

	AddDefaultInteractions();

	AddHighlightTargetRecursive(this);

	TArray<UPrimitiveComponent*> AllPrimitiveComps;
	GetComponents<UPrimitiveComponent>(AllPrimitiveComps, true);
	for (UPrimitiveComponent* Comp : AllPrimitiveComps)
	{
		if (IsValid(Comp))
		{
			if (Comp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldStatic || Comp->GetCollisionObjectType() == ECollisionChannel::ECC_WorldDynamic)
			{
				Comp->SetCollisionProfileName(TEXT("FacilityMesh"));	
			}
		}
	}
}

void AFacility::ConnectFacilityAddon(AFacilityAddon* Addon)
{
	ConnectedAddons.AddUnique(Addon);
	Addon->OnConnectedToFacility();
}

FText AFacility::GetFacilityName() const
{
	return IsValid(FacilityData) ? FacilityData->FacilityName : FText::FromString(TEXT("FacilityName_None"));
}

FVector AFacility::GetFacilityMeshTop() const
{
	FVector TraceStart = GetLocationOnTopXYPlane() - FVector::ZAxisVector;
	FVector TraceEnd = TraceStart - FVector::ZAxisVector * GRID_CELL_SIZE;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel2));
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		TraceStart,
		TraceEnd,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		OutHit,
		false
	);

	return OutHit.ImpactPoint;
}

void AFacility::AddDefaultInteractions()
{
	InteractionComponent->AddInteractionDynamic(GetFacilityName().ToString(), this, &AFacility::OnShowDetailInteraction);
}

void AFacility::OnShowDetailInteraction(AActor* Interactor)
{
	if (DetailWindowClass)
	{
		if (UGSBWindowSubsystem* WindowManager = UGSBWindowSubsystem::Get(this))
		{
			DetailWindow = Cast<UGSBFacilityDetailWindow>(WindowManager->OpenWindow(DetailWindowClass, FName(GetFacilityName().ToString() + TEXT(" Detail Window"))));
			if (DetailWindow)
			{
				DetailWindow->OnLinkedToFacility(this);
			}
			else
			{
				TRACE_SCREEN_LOG(TEXT("UGSBFacilityDetailWindow 캐스팅 실패.") + GetFacilityName().ToString());
			}
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("DetailWindowClass가 nullptr입니다. :") + GetFacilityName().ToString());
	}
}

void AFacility::AddHighlightTargetRecursive(AActor* Actor)
{
	if (!IsValid(Actor))
	{
		return;
	}

	TArray<UMeshComponent*> AllMeshComponents;
	Actor->GetComponents<UMeshComponent>(AllMeshComponents);
	for (UMeshComponent* MeshComp : AllMeshComponents)
	{
		MeshOverlayHelperComponent->AddHighlightTarget(MeshComp);
	}

	TArray<AActor*> AllChildActors;
	Actor->GetAllChildActors(AllChildActors);
	for (AActor* ChildActor : AllChildActors)
	{
		if (!IsValid(ChildActor))
		{
			continue;
		}
		else
		{
			AddHighlightTargetRecursive(ChildActor);
		}
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
