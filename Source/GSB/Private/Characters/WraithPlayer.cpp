// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/WraithPlayer.h"
#include "DebugHeader.h"

AWraithPlayer::AWraithPlayer()
{
}

void AWraithPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AWraithPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

void AWraithPlayer::BeginPlay()
{
	Super::BeginPlay();
}

void AWraithPlayer::Ability1_Started()
{
	// TODO
	SCREEN_LOG_NONE_KEY(TEXT("Ability1"));
}

void AWraithPlayer::Ability2_Started()
{
	// TODO
	SCREEN_LOG_NONE_KEY(TEXT("Ability2"));
}

void AWraithPlayer::Ability3_Started()
{
	// TODO
	SCREEN_LOG_NONE_KEY(TEXT("Ability3"));
}
