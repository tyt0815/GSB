// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/TopDownBuildPawn.h"
#include "BuildSystem/FacilityBuilder.h"
#include "Camera/CameraComponent.h"
#include "Characters/GSBPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerController/GSBPlayerController.h"
#include "PlayerController/GSBPlayerInputActionSetDataAsset.h"
#include "HUDs/GSBPlayerOverlay.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "DebugHeader.h"

ATopDownBuildPawn::ATopDownBuildPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SetRootComponent(SpringArm);
	SpringArm->TargetArmLength = 300;
	SpringArm->TargetOffset = FVector(SpringArm->TargetArmLength, 0, SpringArm->TargetArmLength);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeRotation(FRotator(-90, 0, 0));

	FloatingPawnMovement = CreateDefaultSubobject<UFloatingPawnMovement>(TEXT("FloatingPawnMovement"));
}

void ATopDownBuildPawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (IsControlled())
	{
		UpdateFacilityBuilderLocation();
	}
}

void ATopDownBuildPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AGSBPlayerController* PlayerController = GetController<AGSBPlayerController>())
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			if (const UGSBPlayerInputActionSetDataAsset* InputSet = PlayerController->GetInputSet())
			{
				EnhancedInputComponent->BindAction(InputSet->IA_Move, ETriggerEvent::Triggered, this, &ATopDownBuildPawn::Move);
				EnhancedInputComponent->BindAction(InputSet->IA_ToggleTopDownAndThirdPersonBuildMode, ETriggerEvent::Started, this, &ATopDownBuildPawn::SwitchToThirdPersonBuildMode);
			}
		}
	}
}

void ATopDownBuildPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ATopDownBuildPawn::IsControlled() const
{
	return GetController<AGSBPlayerController>() != nullptr;
}

void ATopDownBuildPawn::OnEnterTopDownBuildModeGameAndUI()
{
	if (IsValid(OwningPlayer))
	{
		SetActorLocation(OwningPlayer->GetActorLocation() + FVector::ZAxisVector * 2000);
	}
	
	if (IsValid(PlayerOverlay))
	{
		PlayerOverlay->SwitchToTopViewModeUI();
	}
}

void ATopDownBuildPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ATopDownBuildPawn::SwitchToThirdPersonBuildMode()
{
	if (IsValid(OwningPlayer))
	{
		if (AGSBPlayerController* PC = GetController<AGSBPlayerController>())
		{
			PC->SwitchGamePlayMode_PlayerBuildGameOnly(OwningPlayer);
		}
	}
}

void ATopDownBuildPawn::UpdateFacilityBuilderLocation()
{
}
