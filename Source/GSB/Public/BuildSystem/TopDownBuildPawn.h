// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Pawn.h"
#include "TopDownBuildPawn.generated.h"

struct FInputActionValue;

UCLASS()
class GSB_API ATopDownBuildPawn : public APawn
{
	GENERATED_BODY()

public:
	ATopDownBuildPawn();

	virtual void Tick(float DeltaTime) override;

	virtual void SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent) override;

protected:
	virtual void BeginPlay() override;

public:	
	void OnPossessedByPlayerController();

protected:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* FloatingPawnMovement;

private:
	void Move(const FInputActionValue& Value);
	void SwitchToCharacterBuildMode();

	class AGSBPlayer* OwningPlayer;

public:
	FORCEINLINE void SetOwningPlayer(class AGSBPlayer* NewPlayer)
	{
		OwningPlayer = NewPlayer;
	}
};
