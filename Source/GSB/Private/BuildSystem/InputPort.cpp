// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/InputPort.h"

void AInputPort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	TryConnectToItemSender();
	TryReceiveItemFrom(GetItemSender().GetInterface());

	Tick_ItemAlongSpline(DeltaTime);
}
