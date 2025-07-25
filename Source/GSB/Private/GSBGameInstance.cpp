// Fill out your copyright notice in the Description page of Project Settings.


#include "GSBGameInstance.h"
#include "DebugHeader.h"

UGSBGameInstance* UGSBGameInstance::Get(UObject* WorldContext)
{
	if (IsValid(WorldContext))
	{
		if (UWorld* World = WorldContext->GetWorld())
		{
			return Cast<UGSBGameInstance>(World->GetGameInstance());
		}
	}
	
	return nullptr;
}

TSubclassOf<AActor> UGSBGameInstance::GetActorClass(UObject* WorldContext, const FName& Name)
{
	if (UGSBGameInstance* GameInst = UGSBGameInstance::Get(WorldContext))
	{
		return GameInst->GetActorClass(Name);
	}
	return nullptr;
}

UCurveFloat* UGSBGameInstance::GetCurveFloat(UObject* WorldContext, const FName& Name)
{
	if (UGSBGameInstance* GameInst = UGSBGameInstance::Get(WorldContext))
	{
		return GameInst->GetCurveFloat(Name);
	}
	return nullptr;
}

TSubclassOf<AActor> UGSBGameInstance::GetActorClass(const FName& Name) const
{
	if (ActorClasses.Contains(Name))
	{
		return ActorClasses[Name];
	}
	TRACE_SCREEN_LOG(TEXT("존재하지 않는 공용 Actor 클래스: ") + Name.ToString());
	return nullptr;
}

TSubclassOf<UUserWidget> UGSBGameInstance::GetUserWidgetClass(const FName& Name) const
{
	if (UserWidgetClasses.Contains(Name))
	{
		return UserWidgetClasses[Name];
	}
	TRACE_SCREEN_LOG(TEXT("존재하지 않는 공용 UserWidget 클래스: ") + Name.ToString());
	return nullptr;
}

UMaterialInterface* UGSBGameInstance::GetMaterialInterface(const FName& Name) const
{
	if (MaterialInterfaces.Contains(Name))
	{
		return MaterialInterfaces[Name];
	}
	TRACE_SCREEN_LOG(TEXT("존재하지 않는 공용 MaterialInterface: ") + Name.ToString());
	return nullptr;
}

UCurveFloat* UGSBGameInstance::GetCurveFloat(const FName& Name) const
{
	if (CurveFloats.Contains(Name))
	{
		return CurveFloats[Name];
	}
	TRACE_SCREEN_LOG(TEXT("존재하지 않는 공용 CurveFloat: ") + Name.ToString());
	return nullptr;
}
