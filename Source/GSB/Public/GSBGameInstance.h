// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/GameInstance.h"
#include "GameFramework/Actor.h"
#include "Blueprint/UserWidget.h"
#include "GSBGameInstance.generated.h"

UCLASS()
class GSB_API UGSBGameInstance : public UGameInstance
{
	GENERATED_BODY()

public:
	TSubclassOf<AActor> GetActorClass(const FName& Name) const;

	TSubclassOf<UUserWidget> GetUserWidgetClass(const FName& Name) const;

	UMaterialInterface* GetMaterialInterface(const FName& Name) const;

	UCurveFloat* GetCurveFloat(const FName& Name) const;

private:
	////////////////////////////////////////////////////////////////////////////////
	// Subclass
	////////////////////////////////////////////////////////////////////////////////
	UPROPERTY(EditDefaultsOnly)
	TMap<FName, TSubclassOf<AActor>> ActorClasses;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, TSubclassOf<UUserWidget>> UserWidgetClasses;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, UMaterialInterface*> MaterialInterfaces;

	UPROPERTY(EditDefaultsOnly)
	TMap<FName, UCurveFloat*> CurveFloats;
};