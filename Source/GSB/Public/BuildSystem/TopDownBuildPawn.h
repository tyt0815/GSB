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
	bool IsControlled() const;

	void OnEnterTopDownBuildModeGameAndUI();

protected:
	UPROPERTY(VisibleAnywhere)
	class USpringArmComponent* SpringArm;

	UPROPERTY(VisibleAnywhere)
	class UCameraComponent* Camera;

	UPROPERTY(VisibleAnywhere)
	class UFloatingPawnMovement* FloatingPawnMovement;

private:
	void Move(const FInputActionValue& Value);

	void SwitchToThirdPersonBuildMode();

	void UpdateFacilityBuilderLocation();

	class AGSBPlayer* OwningPlayer;

	class AFacilityBuilder* FacilityBuilder;

	class UGSBPlayerOverlay* PlayerOverlay;

public:
	FORCEINLINE void SetOwningPlayer(class AGSBPlayer* NewPlayer)
	{
		OwningPlayer = NewPlayer;
	}
	FORCEINLINE void SetFacilityBuilder(class AFacilityBuilder* NewFacilityBuilder)
	{
		FacilityBuilder = NewFacilityBuilder;
	}
	FORCEINLINE void SetPlayerOverlay(class UGSBPlayerOverlay* NewPlayerOverlay)
	{
		PlayerOverlay = NewPlayerOverlay;
	}

};
