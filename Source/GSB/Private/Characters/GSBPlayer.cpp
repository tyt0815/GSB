// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GSBPlayer.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "PlayerController/GSBPlayerController.h"
#include "Actors/FacilityBuilder.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GSBGameInstance.h"
#include "DebugHeader.h"

AGSBPlayer::AGSBPlayer()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;		// 카메라 회전
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	// 캐릭터 가속방향으로 캐릭터 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AGSBPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (GamePlayMode == EGamePlayMode::EGPM_Build && FacilityBuilder->IsPlacingFacility())
	{
		UpdateFacilityBuilderLocation();
	}
}

void AGSBPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent);

	if (EnhancedInputComponent)
	{
		// Default
		EnhancedInputComponent->BindAction(MoveInputAction, ETriggerEvent::Triggered, this, &AGSBPlayer::Move);
		EnhancedInputComponent->BindAction(LookInputAction, ETriggerEvent::Triggered, this, &AGSBPlayer::Look);
		EnhancedInputComponent->BindAction(JumpInputAction, ETriggerEvent::Triggered, this, &AGSBPlayer::Jump);
		EnhancedInputComponent->BindAction(ToggleCombatAndBuildModeInputAction, ETriggerEvent::Started, this, &AGSBPlayer::ToggleCombatAndBuildMode);

		// Build Mode
		EnhancedInputComponent->BindAction(ConfirmFacilityPlacementInputAction, ETriggerEvent::Started, this, &AGSBPlayer::ConfirmFacilityPlacement);
		EnhancedInputComponent->BindAction(CancelFacilityPreviewInputAction, ETriggerEvent::Started, this, &AGSBPlayer::CancelFacilityPreview);
		for (int i = 0; i < SelectFacilityTypeInputActions.Num(); ++i)
		{
			if (SelectFacilityTypeInputActions[i])
			{
				EnhancedInputComponent->BindActionValueLambda(SelectFacilityTypeInputActions[i], ETriggerEvent::Started,
					[this, i](const FInputActionValue& InputActionValue)
					{
						SelectFacilityType(i);
					}
				);
			}
		}

		// Combat Mode
		EnhancedInputComponent->BindAction(Ability1InputAction, ETriggerEvent::Started, this, &AGSBPlayer::Ability1_Started);
		EnhancedInputComponent->BindAction(Ability2InputAction, ETriggerEvent::Started, this, &AGSBPlayer::Ability2_Started);
		EnhancedInputComponent->BindAction(Ability3InputAction, ETriggerEvent::Started, this, &AGSBPlayer::Ability3_Started);
	}
}

void AGSBPlayer::BeginPlay()
{
	Super::BeginPlay();

	// InputMapping Context 추가
	AGSBPlayerController* PlayerController = Cast<AGSBPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->AddInputMappingContext(DefaultInputMapping);
		PlayerController->AddInputMappingContext(CombatModeInputMapping);
	}
	else
	{
		SCREEN_LOG_NONE_KEY(TEXT("AGSBPlayerController를 캐스팅 실패(AGSBPlayer::BeginPlay)"));
	}

	// Facility Builder 생성
	UGSBGameInstance* GameInstance = Cast<UGSBGameInstance>(GetGameInstance());
	UWorld* World = GetWorld();
	if (GameInstance && World)
	{
		FacilityBuilder = World->SpawnActor<AFacilityBuilder>(GameInstance->GetDefaultFacilityBuilderClass());
	}
	if (!FacilityBuilder)
	{
		SCREEN_LOG_NONE_KEY(TEXT("FacilityBuilder 스폰 실패(AGSBPlayer::BeginPlay)"));
	}
}

void AGSBPlayer::Move(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D MovementVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// find out which way is forward
		const FRotator Rotation = Controller->GetControlRotation();
		const FRotator YawRotation(0, Rotation.Yaw, 0);

		// get forward vector
		const FVector ForwardDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::X);

		// get right vector 
		const FVector RightDirection = FRotationMatrix(YawRotation).GetUnitAxis(EAxis::Y);

		// add movement 
		AddMovementInput(ForwardDirection, MovementVector.Y);
		AddMovementInput(RightDirection, MovementVector.X);
	}
}

void AGSBPlayer::Look(const FInputActionValue& Value)
{
	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGSBPlayer::ToggleCombatAndBuildMode()
{
	switch (GamePlayMode)
	{
	case EGamePlayMode::EGPM_Combat:
		SetGamePlayMode(EGamePlayMode::EGPM_Build);
		break;
	case EGamePlayMode::EGPM_Build:
		FacilityBuilder->CancelFacilityPreview();
		SetGamePlayMode(EGamePlayMode::EGPM_Combat);
		break;
	default:
		SCREEN_LOG_NONE_KEY(TEXT("정의 되지 않은 case(AGSBPlayer::ToggleCombatAndBuildMode)"));
		break;
	}
}

void AGSBPlayer::ConfirmFacilityPlacement()
{
	FacilityBuilder->ConfirmFacilityPlacement();
}

void AGSBPlayer::CancelFacilityPreview()
{
	FacilityBuilder->CancelFacilityPreview();
}

void AGSBPlayer::SelectFacilityType(uint32 Index)
{
	if (TestFacilityClasses.IsValidIndex(Index))
	{
		FacilityBuilder->PreviewFacility(TestFacilityClasses[Index]);
	}
}

void AGSBPlayer::SetGamePlayMode(EGamePlayMode NewGamePlayMode)
{
	if (GamePlayMode == NewGamePlayMode)
	{
		return;
	}

	GamePlayMode = NewGamePlayMode;
	switch (GamePlayMode)
	{
	case EGamePlayMode::EGPM_Combat:
		SetGamePlayMode_Combat();
		break;
	case EGamePlayMode::EGPM_Build:
		SetGamePlayMode_Build();
		break;
	default:
		SCREEN_LOG_NONE_KEY(TEXT("Undefined GamePlayMode: ") + FString::FromInt(static_cast<int32>(GamePlayMode)) + TEXT("(AGSBPlayer::SetGamePlayMode)"));
		break;
	}
}

void AGSBPlayer::SetGamePlayMode_Combat()
{
	SCREEN_LOG_NONE_KEY(TEXT("CombatMode(AGSBPlayer::SetGamePlayMode_Combat)"));

	AGSBPlayerController* PlayerController = Cast<AGSBPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->RemoveInputMappingContext(BuildModeInputMapping);
		PlayerController->AddInputMappingContext(CombatModeInputMapping);
	}
}

void AGSBPlayer::SetGamePlayMode_Build()
{
	SCREEN_LOG_NONE_KEY(TEXT("BuildMode(AGSBPlayer::SetGamePlayMode_Build)"));

	AGSBPlayerController* PlayerController = Cast<AGSBPlayerController>(GetController());
	if (PlayerController)
	{
		PlayerController->RemoveInputMappingContext(CombatModeInputMapping);
		PlayerController->AddInputMappingContext(BuildModeInputMapping);
	}
}

void AGSBPlayer::UpdateFacilityBuilderLocation()
{
	float TraceLength = 2000.0f;
	FVector Start = CameraComponent->GetComponentLocation();
	FVector End = Start + CameraComponent->GetForwardVector() * TraceLength;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectType;
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectType.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TArray<AActor*> ActorsToIgnore;
	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectType,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	FVector FacilityBuilderLocation = End;
	if (HitResult.GetActor())
	{
		FacilityBuilderLocation = HitResult.ImpactPoint;
	}
	else
	{
		TraceLength = FMath::Max(End.Z - GetActorLocation().Z, 10.0f);
		Start = End;
		End = Start - FVector::ZAxisVector * (TraceLength + GetCapsuleComponent()->GetScaledCapsuleHalfHeight() + 10.0f);
		
		UKismetSystemLibrary::LineTraceSingleForObjects(
			this,
			Start,
			End,
			ObjectType,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);

		if (HitResult.GetActor())
		{
			FacilityBuilderLocation = HitResult.ImpactPoint;
		}
	}

	FacilityBuilder->SetActorLocation(FacilityBuilderLocation);
}
