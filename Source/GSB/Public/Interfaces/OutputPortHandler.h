// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "UObject/Interface.h"
#include "OutputPortHandler.generated.h"

class AOutputPort;

// This class does not need to be modified.
UINTERFACE(MinimalAPI)
class UOutputPortHandler : public UInterface
{
	GENERATED_BODY()
};

/**
 * 
 */
class GSB_API IOutputPortHandler
{
	GENERATED_BODY()

	// Add interface functions to this class. This is the class that will be inherited to implement this interface.
public:
	virtual void RegisterOutputPort(AOutputPort* OutputPort) = 0;
};
