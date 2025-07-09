// Fill out your copyright notice in the Description page of Project Settings.


#include "Facility/MiningPoint.h"
#include "Components/InstancedStaticMeshComponent.h"

AMiningPoint::AMiningPoint()
{
	InstancedStaticMeshComponent = CreateDefaultSubobject<UInstancedStaticMeshComponent>(TEXT("InstancesStaticMesh"));
	InstancedStaticMeshComponent->SetupAttachment(GetRootComponent());
	InstancedStaticMeshComponent->SetCollisionProfileName(TEXT("FacilityMesh"));
}

void AMiningPoint::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	InstancedStaticMeshComponent->ClearInstances();
	FMath::RandInit(RandomDrawSeed);
	for (int i = 0; i < RandomDrawNum; ++i)
	{
		FTransform RandomTransform;
		RandomTransform.SetLocation(
			FVector(
				FMath::RandRange(RandomTransformMin.GetLocation().X, RandomTransformMax.GetLocation().X),
				FMath::RandRange(RandomTransformMin.GetLocation().Y, RandomTransformMax.GetLocation().Y),
				FMath::RandRange(RandomTransformMin.GetLocation().Z, RandomTransformMax.GetLocation().Z)
			)
		);
		RandomTransform.SetRotation(
			FRotator(
				FMath::RandRange(RandomTransformMin.Rotator().Pitch, RandomTransformMax.Rotator().Pitch),
				FMath::RandRange(RandomTransformMin.Rotator().Yaw, RandomTransformMax.Rotator().Yaw),
				FMath::RandRange(RandomTransformMin.Rotator().Roll, RandomTransformMax.Rotator().Roll)
			).Quaternion()
		);
		RandomTransform.SetScale3D(
			FVector(
				FMath::RandRange(RandomTransformMin.GetScale3D().X, RandomTransformMax.GetScale3D().X),
				FMath::RandRange(RandomTransformMin.GetScale3D().Y, RandomTransformMax.GetScale3D().Y),
				FMath::RandRange(RandomTransformMin.GetScale3D().Z, RandomTransformMax.GetScale3D().Z)
			)
		);

		InstancedStaticMeshComponent->AddInstance(RandomTransform);
	}
}
