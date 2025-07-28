// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/TopDownBuildPawn.h"
#include "BuildSystem/FacilityBuilder.h"
#include "EnhancedInputComponent.h"
#include "InputMappingContext.h"
#include "Camera/CameraComponent.h"
#include "Characters/GSBPlayer.h"
#include "GameFramework/SpringArmComponent.h"
#include "GameFramework/FloatingPawnMovement.h"
#include "PlayerController/GSBPlayerController.h"
#include "PlayerController/GSBPlayerInputActionSetDataAsset.h"
#include "HUDs/GSBPlayerOverlay.h"
#include "HUDs/GSBContextMenu.h"
#include "HUDs/GSBContextMenuEntry.h"
#include "Kismet/KismetSystemLibrary.h"
#include "SubSystems/GSBWindowSubsystem.h"
#include "DebugHeader.h"

constexpr float TOP_DOWN_BUILD_PAWN_HEIGHT = 2000;

ATopDownBuildPawn::ATopDownBuildPawn()
{
	PrimaryActorTick.bCanEverTick = true;

	PrimaryActorTick.TickGroup = ETickingGroup::TG_PostPhysics;

	SpringArm = CreateDefaultSubobject<USpringArmComponent>(TEXT("SpringArm"));
	SetRootComponent(SpringArm);
	SpringArm->TargetArmLength = 300;
	SpringArm->TargetOffset = FVector(SpringArm->TargetArmLength, 0, SpringArm->TargetArmLength);

	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("Camera"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->SetRelativeRotation(FRotator(-90, 0, 0));
	Camera->ProjectionMode = ECameraProjectionMode::Orthographic;
	Camera->OrthoWidth = 3000;

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
				EnhancedInputComponent->BindAction(InputSet->IA_ToggleConstructibleFacilityListWindow, ETriggerEvent::Started, this, &ATopDownBuildPawn::ToggleBuildableFacilityList);

				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility1, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility1);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility2, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility2);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility3, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility3);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility4, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility4);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility5, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility5);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility6, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility6);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility7, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility7);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility8, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility8);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility9, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility9);
				EnhancedInputComponent->BindAction(InputSet->IA_PreviewFacility0, ETriggerEvent::Started, this, &ATopDownBuildPawn::PreviewFacility0);

				EnhancedInputComponent->BindAction(InputSet->IA_RotatePreview, ETriggerEvent::Started, this, &ATopDownBuildPawn::RotatePreview);
				EnhancedInputComponent->BindAction(InputSet->IA_ConfirmFacilityPlacement, ETriggerEvent::Started, this, &ATopDownBuildPawn::ConfirmFacilityPlacement);
				EnhancedInputComponent->BindAction(InputSet->IA_CancelFacilityPreview, ETriggerEvent::Started, this, &ATopDownBuildPawn::CancelFacilityPreview);

				EnhancedInputComponent->BindAction(InputSet->IA_SelectFacility, ETriggerEvent::Started, this, &ATopDownBuildPawn::SelectFacility);

			}
		}
	}
}

UInventoryComponent* ATopDownBuildPawn::GetInventoryComponent() const
{
	if (IsValid(OwningPlayer))
	{
		return OwningPlayer->GetInventoryComponent();
	}
	return nullptr;
}

void ATopDownBuildPawn::BeginPlay()
{
	Super::BeginPlay();
	
}

bool ATopDownBuildPawn::IsControlled() const
{
	return GetController<AGSBPlayerController>() != nullptr;
}

void ATopDownBuildPawn::OnEnterTopViewExploreMode()
{
	if (IsValid(PlayerOverlay))
	{
		PlayerOverlay->SwitchToTopViewModeUI();
	}
	if (IsValid(OwningPlayer))
	{
		SetActorLocation(OwningPlayer->GetActorLocation() + FVector::ZAxisVector * TOP_DOWN_BUILD_PAWN_HEIGHT);
	}
}

void ATopDownBuildPawn::OnEnterTopViewBuildMode()
{
	
}

void ATopDownBuildPawn::OnEnterTopViewWindowHandleMode()
{
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
			CancelFacilityPreview();
			PC->SwitchGamePlayMode_PlayerBuildGameOnly(OwningPlayer);
		}
	}
}

void ATopDownBuildPawn::ToggleBuildableFacilityList()
{
	if (IsValid(FacilityBuilder))
	{
		CancelFacilityPreview();
		FacilityBuilder->ToggleBuildableFacilityList();
	}
}

void ATopDownBuildPawn::RotatePreview()
{
	FacilityBuilder->RotatePreview();
}

void ATopDownBuildPawn::ConfirmFacilityPlacement()
{
	FacilityBuilder->ConfirmFacilityPlacement();
}

void ATopDownBuildPawn::CancelFacilityPreview()
{
	if (AGSBPlayerController* PC = GetController<AGSBPlayerController>())
	{
		PC->SetGamePlayMode_TopViewExplore();
	}
	FacilityBuilder->CancelPreview();
}

void ATopDownBuildPawn::PreviewFacility(int32 Index)
{
	if (AGSBPlayerController* PC = GetController<AGSBPlayerController>())
	{
		PC->SetGamePlayMode_TopViewBuild();
	}
	FacilityBuilder->PreviewFacilityAt(Index);
}

void ATopDownBuildPawn::PreviewFacility1()
{
	PreviewFacility(1);
}

void ATopDownBuildPawn::PreviewFacility2()
{
	PreviewFacility(2);
}

void ATopDownBuildPawn::PreviewFacility3()
{
	PreviewFacility(3);
}

void ATopDownBuildPawn::PreviewFacility4()
{
	PreviewFacility(4);
}

void ATopDownBuildPawn::PreviewFacility5()
{
	PreviewFacility(5);
}

void ATopDownBuildPawn::PreviewFacility6()
{
	PreviewFacility(6);
}

void ATopDownBuildPawn::PreviewFacility7()
{
	PreviewFacility(7);
}

void ATopDownBuildPawn::PreviewFacility8()
{
	PreviewFacility(8);
}

void ATopDownBuildPawn::PreviewFacility9()
{
	PreviewFacility(9);
}

void ATopDownBuildPawn::PreviewFacility0()
{
	PreviewFacility(0);
}

void ATopDownBuildPawn::SelectFacility()
{
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_GameTraceChannel1));
	TArray<AActor*> ActorsToIgnore;
	FHitResult OutHit;
	LineTraceSingleToMouseDown(ObjectTypes, ActorsToIgnore, OutHit);

	if (AFacility* Facility = Cast<AFacility>(OutHit.GetActor()))
	{
		OpenFacilityInteractionContextMenu(Facility);
	}
}

void ATopDownBuildPawn::GetMouseWorldPosition(FVector& WorldLocation, FVector& WorldDirection)
{
	if (AGSBPlayerController* PC = GetController<AGSBPlayerController>())
	{
		PC->DeprojectMousePositionToWorld(WorldLocation, WorldDirection);
	}
}

void ATopDownBuildPawn::LineTraceSingleToMouseDown(
	const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
	const TArray<AActor*>& ActorsToIgnore,
	FHitResult& OutHit
)
{
	FVector MouseWorldPosition;
	FVector MouseWorldDirection;
	GetMouseWorldPosition(MouseWorldPosition, MouseWorldDirection);

	FVector LineTraceEnd = MouseWorldPosition + MouseWorldDirection * TOP_DOWN_BUILD_PAWN_HEIGHT * 10;
	UKismetSystemLibrary::LineTraceSingleForObjects(
		this,
		MouseWorldPosition,
		LineTraceEnd,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		OutHit,
		true
	);
}

void ATopDownBuildPawn::OpenFacilityInteractionContextMenu(AFacility* Facility)
{
	if (!IsValid(Facility))
	{
		return;
	}

	if (UGSBWindowSubsystem* WindowManager = UGSBWindowSubsystem::Get(this))
	{
		if (UGSBContextMenu* ContextMenu = WindowManager->OpenDefaultContextMenu(Facility, FName(Facility->GetFacilityName().ToString() + TEXT(": FacilityInteractionContextMenu"))))
		{
			TArray<FString> InteractionDescriptions;
			Facility->SetHighlighInteractableActor(true);
			Facility->GetInteractionDescriptions(InteractionDescriptions);
			ContextMenu->OnClosed.AddDynamic(this, &ATopDownBuildPawn::HandleOnFacilityInteractionContextMenuClosed);
			for (const FString& Description : InteractionDescriptions)
			{
				UGSBContextMenuEntry* Entry = ContextMenu->AddContextMenuEntry(Description);
				Entry->OnClicked.AddDynamic(this, &ATopDownBuildPawn::HandleOnFacilityInteractionContextMenuEntryClicked);
			}
		}
	}
}

void ATopDownBuildPawn::HandleOnFacilityInteractionContextMenuClosed(UGSBContextMenu* Menu)
{
	if (AFacility* Facility = Cast<AFacility>(Menu->GetContextTarget()))
	{
		Facility->SetHighlighInteractableActor(false);
	}
}

void ATopDownBuildPawn::HandleOnFacilityInteractionContextMenuEntryClicked(UGSBContextMenuEntry* Entry)
{
	if (IsValid(Entry))
	{
		if (AFacility* Facility = Cast<AFacility>(Entry->GetContextTarget()))
		{
			Facility->InteractionByDescription(Entry->GetEntryDescriptionText().ToString(), OwningPlayer);
		}
	}
	Entry->CloseContextMenu();
}

void ATopDownBuildPawn::UpdateFacilityBuilderLocation()
{
	if (!IsValid(FacilityBuilder))
	{
		return;
	}
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldDynamic));
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(OwningPlayer);
	ActorsToIgnore.Add(FacilityBuilder);
	FHitResult OutHit;
	LineTraceSingleToMouseDown(ObjectTypes, ActorsToIgnore, OutHit);

	FacilityBuilder->SetActorLocation(OutHit.ImpactPoint);
}
