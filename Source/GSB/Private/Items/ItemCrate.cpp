// Fill out your copyright notice in the Description page of Project Settings.


#include "Items/ItemCrate.h"

// Sets default values
AItemCrate::AItemCrate()
{
	PrimaryActorTick.bCanEverTick = false;

	StaticMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Crate"));
}

// Called when the game starts or when spawned
void AItemCrate::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AItemCrate::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

