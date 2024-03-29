﻿// Fill out your copyright notice in the Description page of Project Settings.


#include "SniperBuilding.h"
// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicBuilding.h"
#include "Kismet/GameplayStatics.h"
#include "Particles/ParticleSystemComponent.h"

// Sets default values
ASniperBuilding::ASniperBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildingName = FString("SniperBuilding");

	RecoilDirection = -1;

	// Import stats from building data table
	Super::SetupStats();
}

FVector ASniperBuilding::GetSearchPosition()
{
	return GetActorLocation() + GetActorUpVector() * 100;
}

// Called when the game starts or when spawned
void ASniperBuilding::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASniperBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASniperBuilding::Build(float DeltaTime)
{
	Super::Build(DeltaTime);
}

void ASniperBuilding::Attack(float DeltaTime)
{
	Super::Attack(DeltaTime);
	
	// Attack target if we can and there is one
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (CurrentTime - LastAttackTime > AttackSpeed)
	{
		if (!IsValid(CurrentTarget))
		{
			CurrentTarget = nullptr;
			SetBuildingState(EBuildingState::BS_Idle);
			return;
		}
		
		if(GetTargetBarrelAngleDifference() > MaxAttackAngleDeviation)
			return;
		
		PlayFireAnimation();
		
		CurrentTarget->LastDamageDealer = BuildingOwner;
		CurrentTarget->DecrementHealth(AttackDamage);
		
		LastAttackTime = GetWorld()->GetTimeSeconds();
	}
}

void ASniperBuilding::CheckForNewTarget()
{
	Super::CheckForNewTarget();
}

