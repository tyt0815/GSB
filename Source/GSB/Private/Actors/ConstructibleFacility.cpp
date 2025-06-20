// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/ConstructibleFacility.h"
#include "Interfaces/PowerProviderFacility.h"
#include "DebugHeader.h"

void AConstructibleFacility::Tick(float Delta)
{
	Super::Tick(Delta);

	switch (FacilityState)
	{
	case EFacilityState::EFS_Preview:
		break;
	case EFacilityState::EFS_Constructing:
		Tick_Constructing(Delta);
		break;
	case EFacilityState::EFS_Running:
		Tick_Running(Delta);
		break;
	case EFacilityState::EFS_Waiting:
		Tick_Waiting(Delta);
		break;
	case EFacilityState::EFS_PowerOff:
		break;
	default:
		SCREEN_LOG_NONE_KEY(TEXT("정의 되지 않은 case(AFacility::Tick)"));
		break;
	}
}

bool AConstructibleFacility::TrySetFacilityState(const EFacilityState& NewFacilityState)
{
	switch (NewFacilityState)
	{
	case EFacilityState::EFS_Preview:
		return TryEnterPreviewState();
		break;
	case EFacilityState::EFS_Constructing:
		return TryEnterConstructingState();
		break;
	case EFacilityState::EFS_Running:
		return TryEnterRunningState();
		break;
	case EFacilityState::EFS_Waiting:
		return TryEnterWaitingState();
		break;
	case EFacilityState::EFS_PowerOff:
		return TryEnterPowerOffState();
		break;
	default:
		SCREEN_LOG_NONE_KEY(TEXT("정의 되지 않은 case(AConstructibleFacility::TrySetFacilityState)"));
		return false;
	}
}

bool AConstructibleFacility::TryEnterPreviewState()
{
	if (CanEnterPreviewState())
	{
		FacilityState = EFacilityState::EFS_Preview;
		OnEnterPreviewState();
		return true;
	}
	return false;
}

bool AConstructibleFacility::TryEnterConstructingState()
{
	if (CanEnterConstructingState())
	{
		FacilityState = EFacilityState::EFS_Constructing;
		OnEnterConstructingState();
		return true;
	}
	return false;
}

bool AConstructibleFacility::TryEnterRunningState()
{
	if (CanEnterRunningState())
	{
		FacilityState = EFacilityState::EFS_Running;
		OnEnterRunningState();
		return true;
	}
	return false;
}

bool AConstructibleFacility::TryEnterWaitingState()
{
	if (CanEnterWaitingState())
	{
		FacilityState = EFacilityState::EFS_Waiting;
		OnEnterWaitingState();
		return true;
	}
	return false;
}

bool AConstructibleFacility::TryEnterPowerOffState()
{
	if (CanEnterPowerOffState())
	{
		FacilityState = EFacilityState::EFS_PowerOff;
		OnEnterPowerOffState();
		return true;
	}
	return false;
}

bool AConstructibleFacility::CanEnterPreviewState()
{
	return true;
}

bool AConstructibleFacility::CanEnterConstructingState()
{
	return IsValidPlace();
}

bool AConstructibleFacility::CanEnterRunningState()
{
	return true;
}

bool AConstructibleFacility::CanEnterWaitingState()
{
	return true;
}

bool AConstructibleFacility::CanEnterPowerOffState()
{
	return true;
}

void AConstructibleFacility::OnEnterPreviewState_Implementation()
{
	TSet<UActorComponent*> AllActorComponents = GetComponents();
	TArray<UStaticMeshComponent*> AllStaticMeshComponents;
	for (UActorComponent* Component : AllActorComponents)
	{
		UStaticMeshComponent* StaticMeshComponent = Cast<UStaticMeshComponent>(Component);
		if (StaticMeshComponent && !StaticMeshComponent->ComponentHasTag(TEXT("PowerArea")))
		{
			AllStaticMeshComponents.Add(StaticMeshComponent);
		}
	}

	for (UStaticMeshComponent* StaticMeshComponent : AllStaticMeshComponents)
	{
		StaticMeshComponent->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	}

	IPowerProviderFacility* PowerProviderInterface = Cast< IPowerProviderFacility>(this);
	if (PowerProviderInterface)
	{
		PowerProviderInterface->SetPowerInfluenceAreaVisibility(true);
	}
}

void AConstructibleFacility::OnEnterConstructingState_Implementation()
{
	ElapsedConstructionTime = 0.0f;

	IPowerProviderFacility* PowerProviderInterface = Cast< IPowerProviderFacility>(this);
	if (PowerProviderInterface)
	{
		PowerProviderInterface->SetPowerInfluenceAreaVisibility(false);
	}
}

void AConstructibleFacility::OnEnterRunningState_Implementation()
{
	SCREEN_LOG(1, TEXT("RunningState"));
}

void AConstructibleFacility::OnEnterWaitingState_Implementation()
{
}

void AConstructibleFacility::OnEnterPowerOffState_Implementation()
{
	SCREEN_LOG(1, TEXT("StoppedState"));
}

bool AConstructibleFacility::IsValidPlace()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	FHitResult HitResult;
	BoxTraceSingleFromGridBoundsForObjects(ObjectTypes, ActorsToIgnore, HitResult, 10);
	if (HitResult.GetActor())
	{
		return false;
	}
	return true;
}

float AConstructibleFacility::GetConstructionProgress() const
{
	return ElapsedConstructionTime / ConstructionTime;
}

void AConstructibleFacility::Tick_Constructing_Implementation(float Delta)
{
	ElapsedConstructionTime += Delta;
	if (GetConstructionProgress() >= 1.0f)
	{
		if (!TrySetFacilityState(EFacilityState::EFS_Waiting))
		{
			TrySetFacilityState(EFacilityState::EFS_PowerOff);
		}
	}
}

void AConstructibleFacility::Tick_Running_Implementation(float Delta)
{
}

void AConstructibleFacility::Tick_Waiting_Implementation(float Delta)
{
}