// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "GSBPlayerController.generated.h"

class UInputMappingContext;

UCLASS()
class GSB_API AGSBPlayerController : public APlayerController
{
	GENERATED_BODY()
	
public:
	void AddInputMappingContext(UInputMappingContext* InputMappingContext);
	void RemoveInputMappingContext(UInputMappingContext* InputMappingContext);
	void ClearAllInputMappingContext();

};
