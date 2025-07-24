// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ExtensionHub.h"
#include "Facility/CentralHub.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/Addon/ConfigurableOutputPort.h"
#include "Characters/GSBPlayer.h"
#include "Components/RetryPrioritizedActorRequestHandlerComponent.h"
#include "Components/ItemStorageComponent.h"
#include "Items/ItemCrate.h"
#include "SubSystems/GSBFacilitySubsystem.h"
#include "DebugHeader.h"	

AExtensionHub::AExtensionHub()
{
	InputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("InputPortHandler"));
	OutputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("OutputPortHandler"));
}

bool AExtensionHub::TryLinkPowerConsumerFacility(APowerConsumerFacility* PowerConsumerFacility)
{
	AHubLinkedFacility* HubLinkedFacility = Cast<AHubLinkedFacility>(PowerConsumerFacility);
	
	if (IsValid(HubLinkedFacility))
	{
		return Super::TryLinkPowerConsumerFacility(PowerConsumerFacility);
	}
	return false;
}

bool AExtensionHub::CanLinkToPowerProvider(IPowerProviderFacility* PowerProvider)
{
	AExtensionHub* ExtensionHub = Cast<AExtensionHub>(PowerProvider);
	return Super::CanLinkToPowerProvider(PowerProvider) &&
		!(IsValid(ExtensionHub) && ExtensionHub->GetLinkedPowerProvider() == this)
		;
}

UItemStorageComponent* AExtensionHub::GetHubStorageComponent()
{
	if (UGSBFacilitySubsystem* FacilitySubsystem = GetGameInstance()->GetSubsystem<UGSBFacilitySubsystem>())
	{
		return FacilitySubsystem->GetCentralHub()->GetHubStorageComponent();
	}
	return nullptr;
}

void AExtensionHub::RegisterInputPort(AInputPort* InputPort)
{
	InputPortHandler->RegisterActor(InputPort);
	InputPort->OnCanReceiveItemCalled.BindDynamic(this, &AExtensionHub::CanReceiveItem);
}

void AExtensionHub::RegisterOutputPort(AOutputPort* OutputPort)
{
	OutputPortHandler->RegisterActor(OutputPort);
}

void AExtensionHub::BeginPlay()
{
	Super::BeginPlay();

	if (UGameInstance* GameInst = GetGameInstance())
	{
		if (UGSBFacilitySubsystem* FacilityManager = Cast<UGSBFacilitySubsystem>(GameInst->GetSubsystem<UGSBFacilitySubsystem>()))
		{
			FacilityManager->AddHub(this);
		}
	}

	InputPortHandler->OnProcess.BindDynamic(this, &AExtensionHub::TryReceiveItemFromInputPort);
	OutputPortHandler->OnProcess.BindDynamic(this, &AExtensionHub::TrySendItemToOutputPort);
}

bool AExtensionHub::CanReceiveItem(const AInputPort* InputPort)
{
	return InputPort->HasToken();
}

bool AExtensionHub::TryReceiveItemFromInputPort(AActor* Actor)
{
	AInputPort* InputPort = Cast<AInputPort>(Actor);
	check(InputPort);

	AItemCrate* ItemCrate = Cast<AItemCrate>(InputPort->GetPendingItemFromSender());
	if (IsValid(ItemCrate))
	{
		FItemStack ItemStack = {};
		ItemStack.ItemData = ItemCrate->GetItemData();
		ItemStack.Stack = 1;
		if (UItemStorageComponent* StorageComponent = GetHubStorageComponent())
		{
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
		
	}

	return false;
}

bool AExtensionHub::TrySendItemToOutputPort(AActor* Actor)
{
	AConfigurableOutputPort* OutputPort = Cast<AConfigurableOutputPort>(Actor);
	check(OutputPort);
	UItemDataAsset* SelectedItem = OutputPort->GetSelectedItem();
	if (OutputPort->IsConnectedToReceiver())
	{
		if (UItemStorageComponent* StorageComponent = GetHubStorageComponent())
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
	}

	return false;
}
