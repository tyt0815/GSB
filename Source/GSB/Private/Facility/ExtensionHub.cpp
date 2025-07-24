// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ExtensionHub.h"
#include "Facility/CentralHub.h"
#include "Characters/GSBPlayer.h"
#include "SubSystems/GSBFacilitySubsystem.h"
#include "HUDs/GSBHubDetailWindow.h"
#include "DebugHeader.h"	

AExtensionHub::AExtensionHub()
{
}

void AExtensionHub::OnShowDetailInteraction(AActor* Interactor)
{
	Super::OnShowDetailInteraction(Interactor);

	if (UGSBHubDetailWindow* Window = Cast<UGSBHubDetailWindow>(DetailWindow))
	{
		Window->LinkHubStorageComponent(GetHubStorageComponent());

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
}
