// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/Addon/ItemPort.h"
#include "Components/SplineTransportComponent.h"

AItemPort::AItemPort()
{
	PrimaryActorTick.bCanEverTick = false;
}


void AItemPort::BeginPlay()
{
	Super::BeginPlay();
}
