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
#include "BuildSystem/TopDownBuildPawn.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GSBGameInstance.h"
#include "HUDs/GSBPlayerHUD.h"
#include "HUDs/GSBPlayerOverlay.h"
#include "HUDs/GSBInventory.h"
#include "HUDs/GSBInventoryWindow.h"
#include "HUDs/GSBItemSlot.h"
#include "HUDs/GSBConstructableFacilityListWindow.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

AGSBPlayer::AGSBPlayer()
{
	SpringArmComponent = CreateDefaultSubobject<USpringArmComponent>(TEXT("SprintArm"));
	SpringArmComponent->SetupAttachment(GetRootComponent());
	SpringArmComponent->bUsePawnControlRotation = true;		// 카메라 회전
	SpringArmComponent->TargetArmLength = 500.0f;
	SpringArmComponent->SetRelativeLocation(FVector(0, 0, 100));
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

	if (IsControlled())
	{
		UpdateFacilityBuilderLocation();

		if (!IsUIMode())
		{
			AActor* Candidate = TraceInteractableActor();
			UpdateInteractableActor(Candidate);
		}
	}
}

void AGSBPlayer::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
	if (AGSBPlayerController* PlayerController = GetPlayerController())
	{
		if (UEnhancedInputComponent* EnhancedInputComponent = Cast<UEnhancedInputComponent>(PlayerInputComponent))
		{
			if (const UGSBPlayerInputActionSetDataAsset* InputSet = PlayerController->GetInputSet())
			{
				EnhancedInputComponent->ClearActionBindings();
				// Locomotion
				EnhancedInputComponent->BindAction(InputSet->IA_Move, ETriggerEvent::Triggered, this, &AGSBPlayer::Move);
				EnhancedInputComponent->BindAction(InputSet->IA_Look, ETriggerEvent::Triggered, this, &AGSBPlayer::Look);
				EnhancedInputComponent->BindAction(InputSet->IA_Jump, ETriggerEvent::Triggered, this, &AGSBPlayer::Jump);

				// Interaction
				EnhancedInputComponent->BindAction(InputSet->IA_Interaction, ETriggerEvent::Started, this, &AGSBPlayer::Interaction);
				EnhancedInputComponent->BindAction(InputSet->IA_SelectInteractionScrollUp, ETriggerEvent::Started, this, &AGSBPlayer::SelectInteractionScrollUp);
				EnhancedInputComponent->BindAction(InputSet->IA_SelectInteractionScrollDown, ETriggerEvent::Started, this, &AGSBPlayer::SelectInteractionScrollDown);

				// Mode Switch
				EnhancedInputComponent->BindAction(InputSet->IA_ToggleCombatAndBuildMode, ETriggerEvent::Started, this, &AGSBPlayer::ToggleCombatAndBuildMode);
				EnhancedInputComponent->BindAction(InputSet->IA_ToggleTopDownAndThirdPersonBuildMode, ETriggerEvent::Started, this, &AGSBPlayer::SwitchToTopDownBuildMode);
				
				// Build System
				EnhancedInputComponent->BindAction(InputSet->IA_RotatePreview, ETriggerEvent::Started, this, &AGSBPlayer::RotatePreview);
				EnhancedInputComponent->BindAction(InputSet->IA_ConfirmFacilityPlacement, ETriggerEvent::Started, this, &AGSBPlayer::ConfirmFacilityPlacement);
				EnhancedInputComponent->BindAction(InputSet->IA_CancelFacilityPreview, ETriggerEvent::Started, this, &AGSBPlayer::CancelFacilityPreview);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility1, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility1);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility2, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility2);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility3, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility3);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility4, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility4);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility5, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility5);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility6, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility6);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility7, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility7);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility8, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility8);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility9, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility9);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility0, ETriggerEvent::Started, this, &AGSBPlayer::PreviewFacility0);

				// Combat
				EnhancedInputComponent->BindAction(InputSet->IA_Ability1, ETriggerEvent::Started, this, &AGSBPlayer::Ability1_Started);
				EnhancedInputComponent->BindAction(InputSet->IA_Ability2, ETriggerEvent::Started, this, &AGSBPlayer::Ability2_Started);
				EnhancedInputComponent->BindAction(InputSet->IA_Ability3, ETriggerEvent::Started, this, &AGSBPlayer::Ability3_Started);

				// UI
				EnhancedInputComponent->BindAction(InputSet->IA_Esc, ETriggerEvent::Started, this, &AGSBPlayer::Esc_Triggered);
				EnhancedInputComponent->BindAction(InputSet->IA_ToggleInventoryWindow, ETriggerEvent::Started, this, &AGSBPlayer::ToggleInventory);
				EnhancedInputComponent->BindAction(InputSet->IA_ToggleConstructibleFacilityListWindow, ETriggerEvent::Started, this, &AGSBPlayer::ToggleBuildableFacilityList);
			}
		}
	}
}

void AGSBPlayer::PostInitializeComponents()
{
	Super::PostInitializeComponents();
}

void AGSBPlayer::BeginPlay()
{
	Super::BeginPlay();

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

	SwitchToCombatMode();

	if (UWorld* World = GetWorld())
	{
		TopDownBuildPawn = World->SpawnActor<ATopDownBuildPawn>();
		TopDownBuildPawn->SetOwningPlayer(this);
		TopDownBuildPawn->SetFacilityBuilder(FacilityBuilder);
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
	if (IsUIMode())
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
		InteractableActor = nullptr;
		if (UGSBPlayerOverlay* OverlayWidget = GetOverlayWidget())
		{
			OverlayWidget->HideInteractionList();
		}
	}
}

void AGSBPlayer::SelectInteractionScrollUp()
{
	if (InteractableActor)
	{
		SelectedInteractionIndex = FMath::Clamp(SelectedInteractionIndex - 1, 0, InteractableActor->GetNumInteractions() - 1);
		if (UGSBPlayerOverlay* OverlayWidget = GetOverlayWidget())
		{
			OverlayWidget->UpdateInteractionFocusing(SelectedInteractionIndex);
		}
	}
}

void AGSBPlayer::SelectInteractionScrollDown()
{
	if (InteractableActor)
	{
		SelectedInteractionIndex = FMath::Clamp(SelectedInteractionIndex + 1, 0, InteractableActor->GetNumInteractions() - 1);
		if (UGSBPlayerOverlay* OverlayWidget = GetOverlayWidget())
		{
			OverlayWidget->UpdateInteractionFocusing(SelectedInteractionIndex);
		}
	}
}

void AGSBPlayer::ToggleInventory()
{
	if (UGSBWindowSubsystem* WindowManager = UGSBWindowSubsystem::Get(this))
	{
		WindowManager->ToggleWindow(InventoryWindow, TEXT("Inventory"), TEXT("Inventory"));
		if (IsValid(InventoryWindow))
		{
			InventoryWindow->OnItemSlotAdded.AddDynamic(this, &AGSBPlayer::OnItemSlotAddedToInventory);
			InventoryWindow->LinkStorageComponent(InventoryComponent);
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

void AGSBPlayer::ToggleBuildableFacilityList()
{
	if (UGSBWindowSubsystem* WindowManager = UGSBWindowSubsystem::Get(this))
	{
		WindowManager->ToggleWindow(ConstructableFacilityListWindow, TEXT("ConstructableFacilityListWindow"), TEXT("ConstructableFacilityListWindow"));
	}
}

void AGSBPlayer::PreviewFacility1()
{
	FacilityBuilder->PreviewFacilityAt(1);
}

void AGSBPlayer::PreviewFacility2()
{
	FacilityBuilder->PreviewFacilityAt(2);
}

void AGSBPlayer::PreviewFacility3()
{
	FacilityBuilder->PreviewFacilityAt(3);
}

void AGSBPlayer::PreviewFacility4()
{
	FacilityBuilder->PreviewFacilityAt(4);
}

void AGSBPlayer::PreviewFacility5()
{
	FacilityBuilder->PreviewFacilityAt(5);
}

void AGSBPlayer::PreviewFacility6()
{
	FacilityBuilder->PreviewFacilityAt(6);
}

void AGSBPlayer::PreviewFacility7()
{
	FacilityBuilder->PreviewFacilityAt(7);
}

void AGSBPlayer::PreviewFacility8()
{
	FacilityBuilder->PreviewFacilityAt(8);
}

void AGSBPlayer::PreviewFacility9()
{
	FacilityBuilder->PreviewFacilityAt(9);
}

void AGSBPlayer::PreviewFacility0()
{
	FacilityBuilder->PreviewFacilityAt(0);
}

void AGSBPlayer::ToggleCombatAndBuildMode()
{
	if (AGSBPlayerController* PlayerController = GetPlayerController())
	{
		if (PlayerController->IsPlayerCombatMode())
		{
			SwitchToBuildMode();
		}
		else
		{
			SwitchToCombatMode();
		}
	}
}

void AGSBPlayer::SwitchToTopDownBuildMode()
{
	if (IsValid(TopDownBuildPawn))
	{
		if (AGSBPlayerController* PC = GetPlayerController())
		{
			PC->SwitchGamePlayMode_TopDownBuildGameAndUI(TopDownBuildPawn);
			GetCharacterMovement()->StopMovementImmediately();
		}
	}
}

void AGSBPlayer::SwitchToCombatMode()
{
	if (AGSBPlayerController* PC = GetPlayerController())
	{
		PC->SwitchGamePlayMode_PlayerCombatGameOnly(this);
	}
}

void AGSBPlayer::SwitchToBuildMode()
{
	if (AGSBPlayerController* PC = GetPlayerController())
	{
		PC->SwitchGamePlayMode_PlayerBuildGameOnly(this);
	}
}

AGSBPlayerController* AGSBPlayer::GetPlayerController() const
{
	return GetController<AGSBPlayerController>();
}

bool AGSBPlayer::IsControlled() const
{
	return GetPlayerController() != nullptr;
}

void AGSBPlayer::OnEnterCombatModeGameOnly()
{
	FacilityBuilder->CancelPreview();

	if (UGSBPlayerOverlay* PlayerOverlay = GetOverlayWidget())
	{
		PlayerOverlay->SwitchToCombatModeUI();
	}
	
}

void AGSBPlayer::OnEnterCombatModeGameAndUI()
{
}

void AGSBPlayer::OnEnterBuildModeGameOnly()
{
	if (UGSBPlayerOverlay* PlayerOverlay = GetOverlayWidget())
	{
		PlayerOverlay->SwitchToBuildModeUI();
	}
}

void AGSBPlayer::OnEnterBuildModeGameAndUI()
{
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
		SelectedInteractionIndex = 0;
		if (UGSBPlayerOverlay* OverlayWidget = GetOverlayWidget())
		{
			OverlayWidget->ShowInteractionList();
			OverlayWidget->UpdateInteractionList(Descriptions);
		}
	}
	else
	{
		if (UGSBPlayerOverlay* OverlayWidget = GetOverlayWidget())
		{
			OverlayWidget->HideInteractionList();
		}
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
	if (AGSBPlayerController* PlayerController = GetPlayerController())
	{
		return PlayerController->bShowMouseCursor;
	}
	return false;
}

void AGSBPlayer::OnItemSlotAddedToInventory(UGSBStorageWindow* Storage, UGSBStorage* StorageBody, UGSBItemList* ItemList, UGSBItemSlot* ItemSlot)
{
	ItemSlot->OnItemSlotLeftClicked.AddDynamic(StorageBody, &UGSBStorage::DropItemByItemSlotWidget);
}

AGSBPlayerHUD* AGSBPlayer::GetHUD() const
{
	if (AGSBPlayerController* PlayerController = GetPlayerController())
	{
		return PlayerController->GetHUD<AGSBPlayerHUD>();
	}
	return nullptr;
}

UGSBPlayerOverlay* AGSBPlayer::GetOverlayWidget() const
{
	if (AGSBPlayerHUD* HUD = GetHUD())
	{
		return Cast<UGSBPlayerOverlay>(HUD->GetOverlayWidget());
	}
	return nullptr;
}