// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Buildings/Building.h"
#include "SniperBuilding.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASniperBuilding : public ABuilding
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASniperBuilding();
	
	virtual FVector GetSearchPosition() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Build(float DeltaTime) override;

	virtual void Attack(float DeltaTime) override;

	virtual void CheckForNewTarget() override;
};
