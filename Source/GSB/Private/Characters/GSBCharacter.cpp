// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GSBCharacter.h"

// Sets default values
AGSBCharacter::AGSBCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

// Called when the game starts or when spawned
void AGSBCharacter::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AGSBCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

// Called to bind functionality to input
void AGSBCharacter::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

}

