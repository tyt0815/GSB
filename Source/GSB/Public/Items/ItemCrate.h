// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "ItemCrate.generated.h"

class UItemDataAsset;

UCLASS()
class GSB_API AItemCrate : public AActor
{
	GENERATED_BODY()
	
public:	
	AItemCrate();
	virtual void Tick(float DeltaTime) override;

protected:
	virtual void BeginPlay() override;

public:	

protected:
	UPROPERTY(VisibleAnywhere)
	UStaticMeshComponent* StaticMeshComponent;

	UPROPERTY(EditAnywhere, Category = "AItemCrate")
	UItemDataAsset* ItemData;
};
