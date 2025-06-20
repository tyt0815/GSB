// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/CentralHub.h"
#include "Actors/PowerConsumerFacility.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

ACentralHub::ACentralHub()
{
	PowerInfluenceAreaComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PowerInfluenceArea"));
	PowerInfluenceAreaComponent->SetupAttachment(GetRootComponent());
	PowerInfluenceAreaComponent->SetCollisionProfileName(TEXT("PowerInfluenceArea"));
	PowerInfluenceAreaComponent->ComponentTags.Add(TEXT("PowerArea"));
}

void ACentralHub::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	UpdatePowerInfluenceAreaScale(GridSize.Z);
}

void ACentralHub::Tick(float Delta)
{
	Super::Tick(Delta);

	SCREEN_LOG_COLOR(22, FColor::Green, TEXT("Power Capacity: ") + FString::FromInt(PowerCapacity) + TEXT("   Current Power Usage: ") + FString::FromInt(GetCurrentPowerUsage()));
}

bool ACentralHub::CanProvidePower()
{
	return PowerCapacity >= GetCurrentPowerUsage();
}

void ACentralHub::BeginPlay()
{
	Super::BeginPlay();
	UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance());
	if (GameInst)
	{
		GameInst->AddHub(this);
	}
}
