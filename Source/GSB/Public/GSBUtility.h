#pragma once

template<typename T>
void FilterComponentsByClass(const TSet<UActorComponent*>& Components, TArray<T*>& FilteredComponents)
{
	for (UActorComponent* Component : Components)
	{
		T* FilteredComponent = Cast<T>(Component);
		if (IsValid(FilteredComponent))
		{
			FilteredComponents.AddUnique(FilteredComponent);
		}
	}
}

template<typename T>
void FilterHitsByClass(const TArray<FHitResult>& HitResults, TArray<FHitResult>& Output)
{
    for (const FHitResult& HitResult : HitResults)
    {
        if (Cast<T>(HitResult.GetActor()))
        {
            Output.Add(HitResult);
        }
    }
}

inline FVector CalcPowerInfluenceAreaCylinderScale(float RadiusInGridUnits, float HeightInGridUnits)
{
	// InGrid -> Scale 매핑
	// InGrid | 실제 반지름 | Scale값
	//   0          50         1.0 
	//   1         150         3.0
	//   2                     5.0f
	float RadiusScale = 1 + RadiusInGridUnits * 2.0f;
	float HeightScale = 1 + HeightInGridUnits * 2.0f;
	return FVector(RadiusScale, RadiusScale, HeightScale);
}