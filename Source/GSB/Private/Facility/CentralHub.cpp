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
#include "HUDs/GSBPlayerHUD.h"
#include "HUDs/GSBPlayerOverlay.h"
#include "HUDs/GSBHubDetailWindow.h"
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

void ACentralHub::OnShowDetailInteraction(AActor* Interactor)
{
	Super::OnShowDetailInteraction(Interactor);

	if (UGSBHubDetailWindow* Window = Cast<UGSBHubDetailWindow>(DetailWindow))
	{
		Window->LinkHubStorageComponent(StorageComponent);

		if (AGSBPlayer* Player = Cast<AGSBPlayer>(Interactor))
		{
			Window->LinkInventoryComponent(Player->GetInventoryComponent());
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("UGSBHubDetailWindow 캐스팅 실패"));
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
	if (Overlay)
	{
		Overlay->UpdatePowerCapacity(PowerProviderComponent->GetCurrentPowerUsage(), PowerCapacity);
	}
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

	if (APlayerController* PlayerController = GetWorld()->GetFirstPlayerController())
	{
		GSBHUD = PlayerController->GetHUD<AGSBHUD>();
		if (GSBHUD)
		{
			SetOverlayWidget();
			if (!Overlay)
			{
				GSBHUD->OnEndBeginPlay.AddDynamic(this, &ACentralHub::SetOverlayWidget);
			}
		}
		else
		{
			TRACE_SCREEN_LOG(TEXT("HUD 캐스팅 실패"));
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("PlayerController 캐스팅 실패"));
	}

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

void ACentralHub::SetOverlayWidget()
{
	Overlay = Cast<UGSBPlayerOverlay>(GSBHUD->GetOverlayWidget());
	UpdatePowerUsage(0);
}