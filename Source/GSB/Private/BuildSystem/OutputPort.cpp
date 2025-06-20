// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/OutputPort.h"

void AOutputPort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

    Tick_ItemAlongSpline(DeltaTime);

    TryConnectItemReceiver();
    TrySendItemTo(GetItemReceiver().GetInterface());
}
