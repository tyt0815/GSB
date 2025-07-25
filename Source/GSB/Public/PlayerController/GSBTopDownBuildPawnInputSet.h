// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Engine/DataAsset.h"
#include "GSBTopDownBuildPawnInputSet.generated.h"

class UInputMappingContext;
class UInputAction;

UCLASS()
class GSB_API UGSBTopDownBuildPawnInputSet : public UDataAsset
{
	GENERATED_BODY()
	
public:
	UPROPERTY(EditDefaultsOnly)
	UInputMappingContext* DefaultInputMappingContext;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* MoveInputAction;

	UPROPERTY(EditDefaultsOnly)
	UInputAction* SwitchToCharacterBuildModeInputAction;
};
