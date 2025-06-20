// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildSystem/ItemPort.h"
#include "Components/SplineComponent.h"
#include "Components/BoxComponent.h"
#include "Components/InstancedStaticMeshComponent.h"

AItemPort::AItemPort()
{
	PrimaryActorTick.bCanEverTick = true;

	GridBoundsComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GridBounds"));
	SetRootComponent(GridBoundsComponent);
	GridBoundsComponent->SetCollisionProfileName(TEXT("GridBounds"));
	GridBoundsComponent->SetBoxExtent(FVector(50.0f));

	SplineComponent = CreateDefaultSubobject<USplineComponent>(TEXT("Spline"));
	SplineComponent->SetupAttachment(GetRootComponent());
	SplineComponent->SetRelativeLocation(FVector(0, 0, -50));
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancedStaticMesh"));
	InstancedStaticMeshComponent->SetupAttachment(GetRootComponent()); 
	InstancedStaticMeshComponent->SetCollisionProfileName(TEXT("FacilityAddon"));
}

void AItemPort::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AItemPort::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InstancedStaticMeshComponent->ClearInstances();

	TArray<FTransform> InstanceTransforms;
	for (int i = 0; i < 10; ++i)
	{
		FTransform InstanceTransform;
		InstanceTransform.SetRotation(FRotator(-90.0f, 0.0f,  0.0f).Quaternion());
		InstanceTransform.SetScale3D(FVector(1.6f));
		InstanceTransform.SetLocation(FVector(40 - i * 10, 0.0f, 0.0f));
		InstanceTransforms.Add(InstanceTransform);
	}

	InstancedStaticMeshComponent->AddInstances(InstanceTransforms, false, false);
}


void AItemPort::BeginPlay()
{
	Super::BeginPlay();
	
	InitializeSplineSpeed();
}

void AItemPort::InitializeSplineSpeed()
{
	SetSplineMoveSpeed(SplineComponent->GetSplineLength());
}
