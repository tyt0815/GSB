// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/ProductionFacility.h"
#include "Components/ItemStorageComponent.h"
#include "Components/RetryPrioritizedActorRequestHandlerComponent.h"
#include "Facility/Addon/InputPort.h"
#include "Facility/Addon/OutputPort.h"
#include "Items/ProductionRecipeSetDataAsset.h"
#include "Items/ItemCrate.h"
#include "HUDs/GSBProductionFacilityDetailWindow.h"
#include "DebugHeader.h"

AProductionFacility::AProductionFacility()
{
	InputStorageComponent = CreateDefaultSubobject<UItemStorageComponent>(TEXT("InputStorage"));
	InputStorageComponent->SetStorageSize(2);
	OutputStorageComponent = CreateDefaultSubobject<UItemStorageComponent>(TEXT("OutputStorage"));
	OutputStorageComponent->SetStorageSize(1);

	InputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("InputPortHandler"));
	OutputPortHandler = CreateDefaultSubobject<URetryPrioritizedActorRequestHandlerComponent>(TEXT("OutputPortHandler"));
	
}

void AProductionFacility::RegisterInputPort(AInputPort* InputPort)
{
	InputPortHandler->RegisterActor(InputPort);
	InputPort->OnCanReceiveItemCalled.BindDynamic(this, &AProductionFacility::CanReceiveItem);
}

void AProductionFacility::RegisterOutputPort(AOutputPort* OutputPort)
{
	OutputPortHandler->RegisterActor(OutputPort);
}

void AProductionFacility::BeginPlay()
{
	Super::BeginPlay();

	InputPortHandler->OnProcess.BindDynamic(this, &AProductionFacility::TryReceiveItemFromInputPort);
	OutputPortHandler->OnProcess.BindDynamic(this, &AProductionFacility::TrySendItemToOutputPort);
}

void AProductionFacility::HandleReceivedItem(const FItemStack& ItemStack)
{
	InputStorageComponent->StoreItem(ItemStack);
	TryStartProduction();
}

float AProductionFacility::GetProductionProgress() const
{
	return ProducingItem == nullptr || GetProductionTime() == 0 ?
		0 : GetWorldTimerManager().GetTimerElapsed(ProducingTimerHandle) / GetProductionTime()
		;
}

float AProductionFacility::GetProductionTime() const
{
	return FMath::Max(0, GetWorldTimerManager().GetTimerRate(ProducingTimerHandle));
}

FProductionRecipe* AProductionFacility::FindProductionRecipe()
{
	if (IsValid(RecipeSet))
	{
		for (FProductionRecipe& Recipe : RecipeSet->Recipes)
		{
			bool bEnoughItem = true;
			for (auto RequiredItem : Recipe.RequiredItems)
			{
				UItemDataAsset* RequiredItemData = RequiredItem.Key;
				int32 RequiredNum = RequiredItem.Value;
				FItemStack ItemStack = InputStorageComponent->GetItemStack(RequiredItemData);
				if (ItemStack.Stack < RequiredNum)
				{
					bEnoughItem = false;
					break;
				}
			}
			if (bEnoughItem)
			{
				return &Recipe;
			}
		}
	}

	return nullptr;
}

bool AProductionFacility::CanStartProduce()
{
	return IsOperating() && ProducingItem == nullptr;
}

bool AProductionFacility::TryReceiveItemFromInputPort(AActor* Actor)
{
	AInputPort* InputPort = Cast<AInputPort>(Actor);
	check(InputPort);

	AItemCrate* ItemCrate = Cast<AItemCrate>(InputPort->GetPendingItemFromSender());
	if (IsValid(ItemCrate))
	{
		FItemStack ItemStack = {};
		ItemStack.ItemData = ItemCrate->GetItemData();
		ItemStack.Stack = 1;
		if (InputStorageComponent->GetAddableStackCount(ItemStack) > 0)
		{
			InputPort->SetHasToken(true);
			bool bReceived = InputPort->TryReceiveItem();
			InputPort->SetHasToken(false);
			if (bReceived)
			{
				InputPort->TakeReceivedItem()->Destroy();
				HandleReceivedItem(ItemStack);
				return true;
			}
		}
	}

	return false;
}

bool AProductionFacility::TrySendItemToOutputPort(AActor* Actor)
{
	AOutputPort* OutputPort = Cast<AOutputPort>(Actor);
	check(OutputPort);

	if (!OutputStorageComponent->IsEmpty() && OutputPort->IsConnectedToReceiver())
	{
		FItemStack ItemStack = OutputStorageComponent->GetItemStackAt(0);
		ItemStack.Stack = 1;
		if (OutputStorageComponent->UnstoreItem(ItemStack) == 1)
		{
			if (OutputPort->TryCreateAndSendItemCrate(ItemStack.ItemData))
			{
				return true;
			}
			else
			{
				OutputStorageComponent->StoreItem(ItemStack);
			}
		}
	}

	return false;
}

bool AProductionFacility::TryStartProduction()
{
	if (CanStartProduce())
	{
		if (GetWorldTimerManager().IsTimerPaused(ProducingTimerHandle))
		{
			GetWorldTimerManager().UnPauseTimer(ProducingTimerHandle);
			return true;
		}

		FProductionRecipe* RecipeInProducing = FindProductionRecipe();
		if (RecipeInProducing)
		{
			// 필요한 아이템 소모
			for (const auto& RequiredItems : RecipeInProducing->RequiredItems)
			{
				FItemStack Required = {};
				Required.ItemData = RequiredItems.Key;
				Required.Stack = RequiredItems.Value;
				InputStorageComponent->UnstoreItem(Required);
			}

			ProducingItem = &RecipeInProducing->ResultItem;

			// 타이머 + 델리게이트로 생산 시작
			FTimerDelegate ProduceDelegate;
			ProduceDelegate.BindUFunction(this, FName("ProduceItem"));
			GetWorldTimerManager().SetTimer(ProducingTimerHandle, ProduceDelegate, FMath::Max(RecipeInProducing->ProductionTime, 0.000001), false);
			return true;
		}
	}
	return false;
}

void AProductionFacility::ProduceItem()
{
	if (ProducingItem)
	{
		OutputStorageComponent->StoreItem(*ProducingItem);
		ProducingItem = nullptr;
	}

	TryStartProduction();
}

bool AProductionFacility::CanReceiveItem(const AInputPort* InputPort)
{
	return InputPort->HasToken();
}
