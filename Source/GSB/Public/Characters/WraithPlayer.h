// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Characters/GSBPlayer.h"
#include "WraithPlayer.generated.h"

/**
 * 
 */
UCLASS()
class GSB_API AWraithPlayer : public AGSBPlayer
{
	GENERATED_BODY()
public:
	AWraithPlayer();
	virtual void Tick(float DeltaTime) override;
	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

	virtual void Ability1_Started() override;
	virtual void Ability2_Started() override;
	virtual void Ability3_Started() override;
};
