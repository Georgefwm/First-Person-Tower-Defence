// Fill out your copyright notice in the Description page of Project Settings.


#include "ABarrierBuilding.h"

ABarrierBuilding::ABarrierBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildingName = FString("BarrierBuilding");

	// Import stats from building data table
	Super::SetupStats();
}

FVector ABarrierBuilding::GetSearchPosition()
{
	return Super::GetSearchPosition();
}

void ABarrierBuilding::BeginPlay()
{
	Super::BeginPlay();
}

void ABarrierBuilding::Tick(float DeltaTime)
{
	switch (CurrentBuildingState)
	{
	case (EBuildingState::BS_Placing):		{ UpdatePreview();							} break;
	case (EBuildingState::BS_Building):		{ Build(DeltaTime);							} break;
	case (EBuildingState::BS_Idle):			{											} break;
	case (EBuildingState::BS_Attacking):	{											} break;
	case (EBuildingState::BS_Disabled):		{											} break;
	default: return;
	}
}

void ABarrierBuilding::Build(float DeltaTime)
{
	Super::Build(DeltaTime);
}

void ABarrierBuilding::Attack(float DeltaTime)
{
}

void ABarrierBuilding::CheckForNewTarget()
{
}
