// Fill out your copyright notice in the Description page of Project Settings.

#include "FireBuilding.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/ModifierEffects/SubTypes/BurnModifier.h"

// Sets default values
AFireBuilding::AFireBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildingName = FString("FireBuilding");

	// Import stats from building data table
	Super::SetupStats();
}

FVector AFireBuilding::GetSearchPosition()
{
	return GetActorLocation() + GetActorUpVector() * 100;
}

// Called when the game starts or when spawned
void AFireBuilding::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AFireBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void AFireBuilding::Build(float DeltaTime)
{
	Super::Build(DeltaTime);
}

void AFireBuilding::Attack(float DeltaTime)
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
		
		// Spawn particles...
		
		CurrentTarget->LastDamageDealer = BuildingOwner;
		CurrentTarget->DecrementHealth(AttackDamage);
		CurrentTarget->GiveModifier(ABurnModifier::StaticClass());
		
		LastAttackTime = GetWorld()->GetTimeSeconds();
	}
}

void AFireBuilding::CheckForNewTarget()
{
	Super::CheckForNewTarget();
}

