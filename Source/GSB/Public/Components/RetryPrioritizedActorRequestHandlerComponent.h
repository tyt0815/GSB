// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/ActorComponent.h"
#include "RetryPrioritizedActorRequestHandlerComponent.generated.h"

DECLARE_DYNAMIC_DELEGATE_RetVal_OneParam(bool, FComponentOnProcessRequestSignature, AActor*, Actor);

UCLASS( ClassGroup=(Custom), meta=(BlueprintSpawnableComponent) )
class GSB_API URetryPrioritizedActorRequestHandlerComponent : public UActorComponent
{
	GENERATED_BODY()

public:	
	URetryPrioritizedActorRequestHandlerComponent();

	virtual void TickComponent(float DeltaTime, ELevelTick TickType, FActorComponentTickFunction* ThisTickFunction) override;

public:
	FComponentOnProcessRequestSignature OnProcess;

	void RegisterActor(AActor* Actor);

	void ProcessRequest();

private:
	TQueue<AActor*> RequestQueue;
};
