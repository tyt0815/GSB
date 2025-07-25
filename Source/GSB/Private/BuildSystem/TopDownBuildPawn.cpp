// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/TopDownBuildPawn.h"
#include "Camera/CameraComponent.h"
#include "Characters/GSBPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerController/GSBPlayerController.h"
#include "PlayerController/GSBTopDownBuildPawnInputSet.h"
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


}

void ATopDownBuildPawn::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	if (AGSBPlayerController* PlayerController = GetController<AGSBPlayerController>())
	{
		PlayerController->ActivateTopDownBuildInputContext();
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			const UGSBTopDownBuildPawnInputSet* InputSet = PlayerController->GetTopDownBuildPawnInputSet();
			// Default
			EnhancedInputComponent->BindAction(InputSet->MoveInputAction, ETriggerEvent::Triggered, this, &ATopDownBuildPawn::Move);
			EnhancedInputComponent->BindAction(InputSet->SwitchToCharacterBuildModeInputAction, ETriggerEvent::Started, this, &ATopDownBuildPawn::SwitchToCharacterBuildMode);
		}
	}

	if (IsValid(OwningPlayer))
	{
		SetActorLocation(OwningPlayer->GetActorLocation() + FVector::ZAxisVector * 2000);
	}
}

void ATopDownBuildPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

void ATopDownBuildPawn::OnPossessedByPlayerController()
{
	if (AGSBPlayerController* PC = GetController<AGSBPlayerController>())
	{
		// PC->ActivateTopDownBuildInputContext();
	}
}

void ATopDownBuildPawn::Move(const FInputActionValue& Value)
{
	FVector2D MovementVector = Value.Get<FVector2D>();

	AddMovementInput(GetActorForwardVector(), MovementVector.Y);
	AddMovementInput(GetActorRightVector(), MovementVector.X);
}

void ATopDownBuildPawn::SwitchToCharacterBuildMode()
{
	if (IsValid(OwningPlayer))
	{
		if (AGSBPlayerController* PC = GetController<AGSBPlayerController>())
		{
			PC->Possess(OwningPlayer);
		}
		else
		{
			TRACE_SCREEN_LOG(TEXT("AGSBPlayerController 캐스팅 실패"));
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("OwningPlayer가 nullptr입니다."));
	}
}
