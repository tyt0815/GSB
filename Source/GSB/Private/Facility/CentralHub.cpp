// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/CentralHub.h"
#include "Facility/PowerConsumerFacility.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/Addon/ConfigurableOutputPort.h"
#include "Components/PowerProviderComponent.h"
#include "Components/RetryPrioritizedActorRequestHandlerComponent.h"
#include "Components/ItemStorageComponent.h"
#include "Items/ItemCrate.h"
#include "Characters/GSBPlayer.h"
#include "HUDs/GSBHubDetailWidget.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

ACentralHub::ACentralHub()
{
	PowerProviderComponent = CreateDefaultSubobject<UPowerProviderComponent>(TEXT("PowerProvider"));
	PowerProviderComponent->SetupAttachment(GetRootComponent());
	PowerProviderComponent->SetCollisionProfileName(TEXT("PowerInfluenceArea"));

	PowerInfluenceAreaStaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerInfluenceArea"));
	PowerInfluenceAreaStaticMeshComponent->SetupAttachment(GetRootComponent());
	PowerInfluenceAreaStaticMeshComponent->SetCollisionProfileName(TEXT("NoCollision"));

	StorageComponent = CreateDefaultSubobject<UItemStorageComponent>(TEXT("Storage"));
	InputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("InputPortHandler"));
	OutputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("OutputPortHandler"));

	FacilityName = TEXT("중앙 허브");
}

void ACentralHub::Tick(float Delta)
{
	Super::Tick(Delta);

	SCREEN_LOG_COLOR(22, FColor::Green, TEXT("Power Capacity: ") + FString::FromInt(PowerCapacity) + TEXT("   Current Power Usage: ") + FString::FromInt(PowerProviderComponent->GetCurrentPowerUsage()));
}

void ACentralHub::OnShowDetailInteraction(AActor* Interactor)
{
	Super::OnShowDetailInteraction(Interactor);

	if (UGSBHubDetailWidget* HubDetailWidget = Cast<UGSBHubDetailWidget>(DetailWidget))
	{
		HubDetailWidget->SetHubStorageAndInventory(GetHubStorageComponent(), Interactor->GetComponentByClass<UItemStorageComponent>());
	}
}

bool ACentralHub::CanProvidePower()
{
	return PowerCapacity >= PowerProviderComponent->GetCurrentPowerUsage();
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
}

void ACentralHub::SetPowerInfluenceAreaVisibility(bool bVisibilty)
{
	PowerProviderComponent->SetPowerInfluenceAreaVisibility(bVisibilty);
}

void ACentralHub::RegisterInputPort(AInputPort* InputPort)
{
	InputPortHandler->RegisterActor(InputPort);
}

void ACentralHub::RegisterOutputPort(AOutputPort* OutputPort)
{
	OutputPortHandler->RegisterActor(OutputPort);
}

UItemStorageComponent* ACentralHub::GetHubStorageComponent()
{
	return StorageComponent;
}

void ACentralHub::BeginPlay()
{
	Super::BeginPlay();
	UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance());
	if (GameInst)
	{
		GameInst->AddHub(this);
	}

	PowerProviderComponent->SetPowerInfluenceAreaMeshComponent(PowerInfluenceAreaStaticMeshComponent);
	PowerProviderComponent->SetPowerInfluenceAreaVisibility(false, false);

	InputPortHandler->OnProcess.BindDynamic(this, &ACentralHub::TryReceiveItemFromInputPort);
	OutputPortHandler->OnProcess.BindDynamic(this, &ACentralHub::TrySendItemToOutputPort);
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
		FItemStack ItemStack;
		ItemStack.ItemData = SelectedItem;
		ItemStack.Stack = 1;
		if (StorageComponent->UnstoreItem(ItemStack) == 1)
		{
			if (OutputPort->TryCreateAndSendItemCrate(ItemStack.ItemData))
			{
				return true;
			}
			else
			{
				StorageComponent->StoreItem(ItemStack);
			}
		}
	}

	return false;
}
