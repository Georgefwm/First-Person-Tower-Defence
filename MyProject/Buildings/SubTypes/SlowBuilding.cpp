// Fill out your copyright notice in the Description page of Project Settings.

#include "SlowBuilding.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/ModifierEffects/SubTypes/MoveSlowModifier.h"

// Sets default values
ASlowBuilding::ASlowBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildingName = FString("SlowBuilding");

	// Import stats from building data table
	SetupStats();
	
	SetRootComponent(BaseModel);
}

FVector ASlowBuilding::GetSearchPosition()
{
	return GetActorLocation() + GetActorUpVector() * 100;
}

// Called when the game starts or when spawned
void ASlowBuilding::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASlowBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlowBuilding::Build(float DeltaTime)
{
	Super::Build(DeltaTime);
}

void ASlowBuilding::Attack(float DeltaTime)
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

		if (FireSound)
			UGameplayStatics::PlaySound2D(GetWorld(), FireSound, 0.2, 1, 0);
		
		// Spawn particles...
		
		CurrentTarget->LastDamageDealer = BuildingOwner;
		CurrentTarget->DecrementHealth(AttackDamage);
		CurrentTarget->GiveModifier(AMoveSlowModifier::StaticClass());
		
		LastAttackTime = GetWorld()->GetTimeSeconds();
	}
}

void ASlowBuilding::CheckForNewTarget()
{
	Super::CheckForNewTarget();
}

