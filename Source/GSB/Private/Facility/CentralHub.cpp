// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/CentralHub.h"
#include "Facility/PowerConsumerFacility.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/Addon/ConfigurableOutputPort.h"
#include "Components/PowerProviderComponent.h"
#include "Components/RetryPrioritizedActorRequestHandlerComponent.h"
#include "Components/ItemStorageComponent.h"
#include "Items/ItemCrate.h"
#include "HUDs/GSBPlayerHUD.h"
#include "HUDs/GSBPlayerOverlay.h"
#include "SubSystems/GSBFacilitySubsystem.h"
#include "DebugHeader.h"

ACentralHub::ACentralHub()
{
	PowerWireConnectionBoundsComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("PowerWireConnectionBounds"));
	PowerWireConnectionBoundsComponent->SetupAttachment(GetRootComponent());
	PowerWireConnectionBoundsComponent->SetCollisionProfileName(TEXT("PowerWireConnectionBounds"));
	PowerWireConnectionBoundsComponent->Mobility = EComponentMobility::Static;

	PowerProviderComponent = CreateDefaultSubobject<UPowerProviderComponent>(TEXT("PowerProvider"));
	PowerProviderComponent->SetupAttachment(GetRootComponent());
	PowerProviderComponent->SetCollisionProfileName(TEXT("PowerInfluenceArea"));
	
	PowerInfluenceAreaStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerInfluenceArea"));
	PowerInfluenceAreaStaticMeshComponent->SetupAttachment(GetRootComponent());
	PowerInfluenceAreaStaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	StorageComponent = CreateDefaultSubobject<UItemStorageComponent>(TEXT("Storage"));
	InputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("InputPortHandler"));
	OutputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("OutputPortHandler"));
}

void ACentralHub::EndPlay(EEndPlayReason::Type EndPlayReason)
{
	PowerProviderComponent->UnlinkAllPowerConsumerFacility();
	Super::EndPlay(EndPlayReason);
}

void ACentralHub::PostInitializeComponents()
{
	Super::PostInitializeComponents();

	if (UGameInstance* GameInst = GetGameInstance())
	{
		if (UGSBFacilitySubsystem* FacilityManager = Cast<UGSBFacilitySubsystem>(GameInst->GetSubsystem<UGSBFacilitySubsystem>()))
		{
			FacilityManager->AddHub(this);
			FacilityManager->SetCentralHub(this);
		}
	}
}

bool ACentralHub::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	return PowerProviderComponent->TryLinkPowerConsumerFacility(PowerConsumerFacility);
}

void ACentralHub::UnlinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	PowerProviderComponent->UnlinkPowerConsumerFacility(PowerConsumerFacility);
}

void ACentralHub::UpdatePowerUsage(int32 Addition)
{
	PowerProviderComponent->UpdatePowerUsage(Addition);
	OnUpdatePowerUsage.Broadcast(PowerProviderComponent->GetCurrentPowerUsage());
}

void ACentralHub::SetPowerInfluenceAreaVisibility(bool bVisibilty)
{
	PowerProviderComponent->SetPowerInfluenceAreaVisibility(bVisibilty);
}

void ACentralHub::RegisterInputPort(AInputPort* InputPort)
{
	InputPortHandler->RegisterActor(InputPort);
	InputPort->OnCanReceiveItemCalled.BindDynamic(this, &ACentralHub::CanReceiveItem);
}

void ACentralHub::RegisterOutputPort(AOutputPort* OutputPort)
{
	OutputPortHandler->RegisterActor(OutputPort);
}

UItemStorageComponent* ACentralHub::GetHubStorageComponent()
{
	return StorageComponent;
}

FVector ACentralHub::GetPowerWireConnectionPoint() const
{
	return PowerWireConnectionBoundsComponent->GetComponentLocation();
}

void ACentralHub::BeginPlay()
{
	Super::BeginPlay();

	PowerProviderComponent->SetPowerInfluenceAreaMeshComponent(PowerInfluenceAreaStaticMeshComponent);
	PowerProviderComponent->SetPowerInfluenceAreaVisibility(false, false);

	InputPortHandler->OnProcess.BindDynamic(this, &ACentralHub::TryReceiveItemFromInputPort);
	OutputPortHandler->OnProcess.BindDynamic(this, &ACentralHub::TrySendItemToOutputPort);

	// Power Usage/Capacity Widget 업데이트용
	UpdatePowerUsage(0);
	UpdatePowerCapacity(0);
}

void ACentralHub::UpdatePowerCapacity(int32 Addition)
{
	PowerCapacity += Addition;
	OnUpdatePowerCapacity.Broadcast(PowerCapacity);
}

bool ACentralHub::CanProvidePower()
{
	return PowerCapacity >= PowerProviderComponent->GetCurrentPowerUsage();
}

bool ACentralHub::TryReceiveItemFromInputPort(AActor* Actor)
{
	AInputPort* InputPort = Cast<AInputPort>(Actor);
	check(InputPort);

	AItemCrate* ItemCrate = Cast<AItemCrate>(InputPort->GetPendingItemFromSender());
	if (IsValid(ItemCrate))
	{
		FItemStack ItemStack = {};
		ItemStack.ItemData = ItemCrate->GetItemData();
		ItemStack.Stack = 1;
		if (StorageComponent->GetAddableStackCount(ItemStack) > 0)
		{
			InputPort->SetHasToken(true);
			bool bReceived = InputPort->TryReceiveItem();
			InputPort->SetHasToken(false);
			if (bReceived)
			{
				InputPort->TakeReceivedItem()->Destroy();
				StorageComponent->StoreItem(ItemStack);
				return true;
			}
		}
	}

	return false;
}

bool ACentralHub::TrySendItemToOutputPort(AActor* Actor)
{
	AConfigurableOutputPort* OutputPort = Cast<AConfigurableOutputPort>(Actor);
	check(OutputPort);
	UItemDataAsset* SelectedItem = OutputPort->GetSelectedItem();
	if (OutputPort->IsConnectedToReceiver())
	{
		if (StorageComponent->GetItemStack(SelectedItem).Stack > 0)
		{
			if (OutputPort->TryCreateAndSendItemCrate(SelectedItem))
			{
				StorageComponent->UnstoreItem({ SelectedItem, 1 });
				return true;
			}
		}
	}

	return false;
}

bool ACentralHub::CanReceiveItem(const AInputPort* InputPort)
{
	return InputPort->HasToken();
}