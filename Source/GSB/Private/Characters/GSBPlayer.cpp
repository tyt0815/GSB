// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/GSBPlayer.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InventoryComponent.h"
#include "PlayerController/GSBPlayerController.h"
#include "PlayerController/GSBPlayerInputActionSetDataAsset.h"
#include "BuildSystem/FacilityBuilder.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GSBGameInstance.h"
#include "HUDs/GSBPlayerHUD.h"
#include "HUDs/GSBPlayerOverlay.h"
#include "HUDs/GSBInventory.h"
#include "HUDs/GSBInventoryWindow.h"
#include "HUDs/GSBItemSlot.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

AGSBPlayer::AGSBPlayer()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;		// 카메라 회전
	CameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	CameraComponent->SetupAttachment(SpringArmComponent);

	InventoryComponent = CreateDefaultSubobject<UInventoryComponent>(TEXT("Inventory"));

	// 캐릭터 가속방향으로 캐릭터 회전
	GetCharacterMovement()->bOrientRotationToMovement = true;
	bUseControllerRotationYaw = false;
}

void AGSBPlayer::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	UpdateFacilityBuilderLocation();

	if (!IsUIMode())
	{
		AActor* Candidate = TraceInteractableActor();
		UpdateInteractableActor(Candidate);
	}
}

void AGSBPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);

	PlayerController = GetController<AGSBPlayerController>();
	if (!PlayerController)
	{
		TRACE_SCREEN_LOG(TEXT("PlayerController 캐스팅 실패"))
		return;
	}

	if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
	{
		const UGSBPlayerInputActionSetDataAsset* InputSet = PlayerController->GetPlayerInputSet();
		// Default
		EnhancedInputComponent->BindAction(InputSet->MoveInputAction, ETriggerEvent::Triggered, this, &AGSBPlayer::Move);
		EnhancedInputComponent->BindAction(InputSet->LookInputAction, ETriggerEvent::Triggered, this, &AGSBPlayer::Look);
		EnhancedInputComponent->BindAction(InputSet->JumpInputAction, ETriggerEvent::Triggered, this, &AGSBPlayer::Jump);
		EnhancedInputComponent->BindAction(InputSet->ToggleInventoryInputAction, ETriggerEvent::Started, this, &AGSBPlayer::ToggleInventory);
		EnhancedInputComponent->BindAction(InputSet->EscInputAction, ETriggerEvent::Started, this, &AGSBPlayer::Esc_Triggered);

		// Interaction
		EnhancedInputComponent->BindAction(InputSet->InteractionInputAction, ETriggerEvent::Started, this, &AGSBPlayer::Interaction);
		EnhancedInputComponent->BindAction(InputSet->SelectInteractionScrollUpInputAction, ETriggerEvent::Started, this, &AGSBPlayer::SelectInteractionScrollUp);
		EnhancedInputComponent->BindAction(InputSet->SelectInteractionScrollDownInputAction, ETriggerEvent::Started, this, &AGSBPlayer::SelectInteractionScrollDown);

		// Build Mode
		EnhancedInputComponent->BindAction(InputSet->RotatePreview, ETriggerEvent::Started, this, &AGSBPlayer::RotatePreview);
		EnhancedInputComponent->BindAction(InputSet->ConfirmFacilityPlacementInputAction, ETriggerEvent::Started, this, &AGSBPlayer::ConfirmFacilityPlacement);
		EnhancedInputComponent->BindAction(InputSet->CancelFacilityPreviewInputAction, ETriggerEvent::Started, this, &AGSBPlayer::CancelFacilityPreview);
		EnhancedInputComponent->BindAction(InputSet->PreviewConveyorBeltInputAction, ETriggerEvent::Started, this, &AGSBPlayer::PreviewConveyorBelt);
		EnhancedInputComponent->BindAction(InputSet->PreviewExtensionHubInputAction, ETriggerEvent::Started, this, &AGSBPlayer::PreviewExtensionHub);
		EnhancedInputComponent->BindAction(InputSet->PreviewMiningFacilityInputAction, ETriggerEvent::Started, this, &AGSBPlayer::PreviewMiningFacility);
		EnhancedInputComponent->BindAction(InputSet->SwitchToCombatModeInputAction, ETriggerEvent::Started, this, &AGSBPlayer::SwitchToCombatMode);

		// Combat Mode
		EnhancedInputComponent->BindAction(InputSet->Ability1InputAction, ETriggerEvent::Started, this, &AGSBPlayer::Ability1_Started);
		EnhancedInputComponent->BindAction(InputSet->Ability2InputAction, ETriggerEvent::Started, this, &AGSBPlayer::Ability2_Started);
		EnhancedInputComponent->BindAction(InputSet->Ability3InputAction, ETriggerEvent::Started, this, &AGSBPlayer::Ability3_Started);
		EnhancedInputComponent->BindAction(InputSet->SwitchToBuildModeInputAction, ETriggerEvent::Started, this, &AGSBPlayer::SwitchToBuildMode);
	}
}

void AGSBPlayer::BeginPlay()
{
	Super::BeginPlay();

	PlayerController = GetController<AGSBPlayerController>();
	if (PlayerController)
	{
		HUD = Cast<AGSBPlayerHUD>(PlayerController->GetHUD());
		if (HUD)
		{
			SetOverlayWidget();
			if (!OverlayWidget)
			{
				HUD->OnEndBeginPlay.AddDynamic(this, &AGSBPlayer::SetOverlayWidget);
			}
		}
		else
		{
			TRACE_SCREEN_LOG(TEXT("HUD 캐스팅 실패"));
		}
		SetGamePlayMode_Combat();
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("AGSBPlayerController 캐스팅 실패"));
	}
	

	if (UGSBGameInstance* GameInst = Cast<UGSBGameInstance>(GetGameInstance()))
	{
		if (UClass* FacilityBuilderClass = GameInst->GetActorClass("FacilityBuilder"))
		{
			FacilityBuilder = GetWorld()->SpawnActor<AFacilityBuilder>(FacilityBuilderClass);
		}
		else
		{
			TRACE_SCREEN_LOG(TEXT("FacilityBuilderClass가 nullptr 입니다."));
		}
		if (!FacilityBuilder)
		{
			FacilityBuilder = GetWorld()->SpawnActor<AFacilityBuilder>();
			TRACE_SCREEN_LOG(TEXT("FacilityBuilder 스폰 실패"));
		}
	}
	else
	{
		TRACE_SCREEN_LOG(TEXT("UGSBGameInstance 캐스팅 실패"));
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
	if (PlayerController->IsUIControlMode())
	{
		return;
	}

	// input is a Vector2D
	FVector2D LookAxisVector = Value.Get<FVector2D>();

	if (Controller != nullptr)
	{
		// add yaw and pitch input to controller
		AddControllerYawInput(LookAxisVector.X);
		AddControllerPitchInput(LookAxisVector.Y);
	}
}

void AGSBPlayer::Interaction()
{
	if (InteractableActor)
	{
		InteractableActor->Interaction(SelectedInteractionIndex, this);
		OverlayWidget->HideInteractionList();
		InteractableActor = nullptr;
	}
}

void AGSBPlayer::SelectInteractionScrollUp()
{
	if (InteractableActor)
	{
		SelectedInteractionIndex = FMath::Clamp(SelectedInteractionIndex - 1, 0, InteractableActor->GetNumInteractions() - 1);
		OverlayWidget->UpdateInteractionFocusing(SelectedInteractionIndex);
	}
}

void AGSBPlayer::SelectInteractionScrollDown()
{
	if (InteractableActor)
	{
		SelectedInteractionIndex = FMath::Clamp(SelectedInteractionIndex + 1, 0, InteractableActor->GetNumInteractions() - 1);
		OverlayWidget->UpdateInteractionFocusing(SelectedInteractionIndex);
	}
}

void AGSBPlayer::ToggleInventory()
{
	UGSBWindowSubsystem* WindowManager = GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>();
	UGSBGameInstance* GameInst = GetGameInstance<UGSBGameInstance>();
	if (WindowManager->IsWindowOpened(InventoryWidget))
	{
		WindowManager->CloseWindow(InventoryWidget);
	}
	else
	{
		UClass* InventoryWidgetClass = GameInst->GetUserWidgetClass(TEXT("Inventory"));
		if (UGSBWindow* Window = WindowManager->OpenWindow(InventoryWidgetClass, TEXT("Inventory")))
		{
			InventoryWidget = Cast<UGSBInventoryWindow>(Window);
			if (InventoryWidget)
			{
				InventoryWidget->OnItemSlotAdded.AddDynamic(this, &AGSBPlayer::OnItemSlotAddedToInventory);
				InventoryWidget->LinkStorageComponent(InventoryComponent);
			}
			else
			{
				TRACE_SCREEN_LOG(TEXT("InventoryWidget 캐스팅 실패"));
				WindowManager->CloseWindow(Window);
			}
		}
		else
		{
			TRACE_SCREEN_LOG(TEXT("InventoryWidget 생성 실패"));
		}
	}
}

void AGSBPlayer::Esc_Triggered()
{
	HideWindow();
}

void AGSBPlayer::HideWindow()
{
	if (IsUIMode())
	{
		GetGameInstance()->GetSubsystem<UGSBWindowSubsystem>()->CloseAllWindows();
	}
}

void AGSBPlayer::RotatePreview()
{
	FacilityBuilder->RotatePreview();
}

void AGSBPlayer::ConfirmFacilityPlacement()
{
	FacilityBuilder->ConfirmFacilityPlacement();
}

void AGSBPlayer::CancelFacilityPreview()
{
	FacilityBuilder->CancelPreview();
}

void AGSBPlayer::PreviewConveyorBelt()
{
	FacilityBuilder->PreviewConveyorBelt();
}

void AGSBPlayer::PreviewExtensionHub()
{
	FacilityBuilder->PreviewGeneralFacility(TEXT("ExtensionHub"));
}

void AGSBPlayer::PreviewMiningFacility()
{
	FacilityBuilder->PreviewMiningFacility();
}

void AGSBPlayer::SwitchToCombatMode()
{
	SetGamePlayMode(EGamePlayMode::EGPM_Combat);
	FacilityBuilder->CancelPreview();
}

void AGSBPlayer::SwitchToBuildMode()
{
	SetGamePlayMode(EGamePlayMode::EGPM_Build);
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
	if (PlayerController)
	{
		PlayerController->ActivateCombatInputContext();
	}

	if (OverlayWidget)
	{
		OverlayWidget->SwitchToCombatModeUI();
	}
}

void AGSBPlayer::SetGamePlayMode_Build()
{
	if (PlayerController)
	{
		PlayerController->ActivateBuildInputContext();
	}

	if (OverlayWidget)
	{
		OverlayWidget->SwitchToBuildModeUI();
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

void AGSBPlayer::UpdateInteractableActor(AActor* Candidate)
{
	if (IsValid(InteractableActor.GetObject()))
	{
		if (InteractableActor.GetObject() == Candidate && InteractableActor->IsInteractable() && !InteractableActor->IsInteractionListDirty())
		{
			return;
		}

		InteractableActor->SetHighlighInteractableActor(false);
	}
	

	InteractableActor = TScriptInterface<IInteractableActor>(Candidate);

	if (InteractableActor)
	{
		InteractableActor->SetHighlighInteractableActor(true);
		TArray<FString> Descriptions;
		InteractableActor->GetInteractionDescriptions(Descriptions);
		InteractableActor->ClearInteractionListDirtyFlag();
		OverlayWidget->ShowInteractionList();
		OverlayWidget->UpdateInteractionList(Descriptions);
		SelectedInteractionIndex = 0;
	}
	else
	{
		OverlayWidget->HideInteractionList();
	}
}

int32 AGSBPlayer::PickupItem(const FItemStack& ItemStack)
{
	return InventoryComponent->StoreItem(ItemStack);
}

AActor* AGSBPlayer::TraceInteractableActor()
{
	float TraceDistance = 500;
	FVector Start = SpringArmComponent->GetComponentLocation();
	FVector End = Start + CameraComponent->GetForwardVector() * TraceDistance;
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel7));
	TArray<AActor*> ActorsToIgnore;

	FHitResult HitResult;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		Start,
		End,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);

	if (!IsValid(HitResult.GetActor()))
	{
		UKismetSystemLibrary::SphereTraceSingleForObjects(
			this,
			Start,
			End,
			100,
			ObjectTypes,
			false,
			ActorsToIgnore,
			EDrawDebugTrace::None,
			HitResult,
			true
		);
	}

	return HitResult.GetActor();
}

bool AGSBPlayer::IsUIMode() const
{
	return PlayerController->bShowMouseCursor;
}

void AGSBPlayer::OnItemSlotAddedToInventory(UGSBStorageWindow* Storage, UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(StorageBody, &UGSBStorage::DropItemByItemSlotWidget);
}

void AGSBPlayer::SetOverlayWidget()
{
	OverlayWidget = Cast<UGSBPlayerOverlay>(HUD->GetOverlayWidget());
}