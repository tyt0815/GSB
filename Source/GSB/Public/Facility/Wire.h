// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Wire.generated.h"

UCLASS()
class GSB_API AWire : public AActor
{
	GENERATED_BODY()
	
public:	
	AWire();

protected:
	virtual void BeginPlay() override;

public:
	UFUNCTION(BlueprintCallable, Category = "GSB|Wire")
	void Connect(const FVector& WorldStart, const FVector& WorldEnd);

	UFUNCTION(BlueprintCallable, Category = "GSB|Wire")
	void Disconnect();

protected:
	UFUNCTION(BlueprintCallable, Category = "GSB|Wire")
	void SetStartAndEndPosition(const FVector& WorldStart, const FVector& WorldEnd);

	UPROPERTY(VisibleAnywhere)
	class USplineMeshComponent* SplineMesh;

	UPROPERTY(VisibleAnywhere)
	class UTimelineComponent* ConnectingTimeline;

	UPROPERTY(VisibleAnywhere)
	class UTimelineComponent* DisconnectingTimeline;

	UPROPERTY(EditAnywhere, Category = "GSB|Wire")
	float DissolveTime = 2;

	UPROPERTY(EditAnywhere, Category = "GSB|Wire")
	float LinkDissolveOffset = 0.2;

	UPROPERTY(EditAnywhere, Category = "GSB|Wire")
	float UnlinkDissolveOffset = 0.5;

private:
	TArray<UMaterialInstanceDynamic*> SplineMeshDynamicMaterialInsts;

	FVector ConvertWorldToLocalPosition(const FVector& WorldPosition);

	void InitializeLinkTimeline();

	void InitializeUnlinkTimeline();

	void DissolveSplineMesh(float Amount);

	UFUNCTION()
	void OnConnecting(float Time);

	UFUNCTION()
	void EndConnecting();

	UFUNCTION()
	void OnDisconnecting(float Time);

	UFUNCTION()
	void EndDisconnecting();
};
