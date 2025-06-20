#include "Actors/Facility.h"
#include "Components/BoxComponent.h"
#include "Kismet/KismetSystemLibrary.h"
#include "GSBDefines.h"
#include "Interfaces/PowerProviderFacility.h"
#include "DebugHeader.h"

AFacility::AFacility()
{
	PrimaryActorTick.bCanEverTick = true;

	GridBoundsComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("GridBounds"));
	SetRootComponent(GridBoundsComponent);
	GridBoundsComponent->SetCollisionProfileName(TEXT("GridBounds"));
}

void AFacility::Tick(float Delta)
{
	Super::Tick(Delta);

}

void AFacility::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);
	GridBoundsComponent->SetBoxExtent(CalcBoxExtentFromGridSize(FVector(GridSize)));
}

void AFacility::BeginPlay()
{
	Super::BeginPlay();

	IPowerProviderFacility* PowerProviderInterface = Cast<IPowerProviderFacility>(this);
	if (PowerProviderInterface)
	{
		PowerProviderInterface->SetPowerInfluenceAreaVisibility(false);
	}
}

void AFacility::SnapActorToGrid(const FVector& NewLocation, bool bSweep, FHitResult* OutSweepHitResult, ETeleportType Teleport)
{
	SetActorLocation(GetSnappedGridLocation(NewLocation), bSweep, OutSweepHitResult, Teleport);
}

FVector AFacility::CalcBoxExtentFromGridSize(FVector InGridSize)
{
	FVector BoxExtent(GRID_CELL_SIZE / 2);
	BoxExtent += FVector(InGridSize) * GRID_CELL_SIZE;
	return BoxExtent;
}

void AFacility::InitializeBoxTraceFromGridBoundsParams(FBoxTraceFromGridBoundsParameters& Params, float Padding)
{
	FVector Up = GetActorUpVector();
	Params.Start = GetActorLocation() + Up * (GridBoundsComponent->GetScaledBoxExtent().Z - Padding);
	Params.End = GetActorLocation() - Up * (GridBoundsComponent->GetScaledBoxExtent().Z - Padding);
	Params.HalfSize.X = GridBoundsComponent->GetScaledBoxExtent().X - Padding;
	Params.HalfSize.Y = GridBoundsComponent->GetScaledBoxExtent().Y - Padding;
	Params.HalfSize.Z = 0.0f;
	Params.Orientation = GetActorRotation();
}

void AFacility::BoxTraceSingleFromGridBoundsForObjects(
	const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes,
	const TArray<AActor*>& ActorsToIgnore,
	FHitResult& HitResult,
	float Padding
)
{
	FBoxTraceFromGridBoundsParameters Params;
	InitializeBoxTraceFromGridBoundsParams(Params, Padding);

	UKismetSystemLibrary::BoxTraceSingleForObjects(
		this,
		Params.Start,
		Params.End,
		Params.HalfSize,
		Params.Orientation,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResult,
		true
	);
}

void AFacility::BoxTraceMultiFromGridBoundsForObjects(const TArray<TEnumAsByte<EObjectTypeQuery>>& ObjectTypes, const TArray<AActor*>& ActorsToIgnore, TArray<FHitResult>& HitResults, float Padding)
{
	FBoxTraceFromGridBoundsParameters Params;
	InitializeBoxTraceFromGridBoundsParams(Params, Padding);

	UKismetSystemLibrary::BoxTraceMultiForObjects(
		this,
		Params.Start,
		Params.End,
		Params.HalfSize,
		Params.Orientation,
		ObjectTypes,
		false,
		ActorsToIgnore,
		EDrawDebugTrace::None,
		HitResults,
		true
	);
}

FVector AFacility::GetSnappedGridLocation(const FVector& Location)
{
	FVector GridLocation = Location;
	int32 HalfSize = GRID_CELL_SIZE / 2;
	GridLocation.X = GridLocation.X > 0 ? GridLocation.X + HalfSize : GridLocation.X - HalfSize;
	GridLocation.X -= static_cast<int64>(GridLocation.X) % GRID_CELL_SIZE;
	GridLocation.Y = GridLocation.Y > 0 ? GridLocation.Y + HalfSize : GridLocation.Y - HalfSize;
	GridLocation.Y -= static_cast<int64>(GridLocation.Y) % GRID_CELL_SIZE;
	GridLocation.Z += GridBoundsComponent->GetScaledBoxExtent().Z;
	return GridLocation;
}

