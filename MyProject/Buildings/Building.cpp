// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/Enemies/Enemy.h"


// Sets default values
ABuilding::ABuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::DontSpawnIfColliding;
}

FVector ABuilding::GetSearchPosition()
{
	return FVector(0.0, 0.0, 0.0);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	LastAttackTime = GetWorld()->GetTimeSeconds() - 100;
}

void ABuilding::CheckBuildingLocation()
{
	
}

void ABuilding::SetBuildingState(EBuildingState State)
{
	CurrentBuildingState = State;
}

void ABuilding::Build(float DeltaTime)
{
	if (BuildStatus >= BuildTime)
	{
		CurrentBuildingState = EBuildingState::BS_Idle;
	}
	
	CurrentHealthPoints += MaxHealthPoints * (DeltaTime / BuildTime);
	BuildStatus += DeltaTime;
}

void ABuilding::Attack(float DeltaTime)
{
}

bool ABuilding::HasLineOfSight(AEnemy* Target)
{
	FVector Location = this->GetSearchPosition();

	FVector From = Location;
	FVector To = Target->GetActorLocation();

	FHitResult HitRes;
	
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TraceParams.bFindInitialOverlaps = true;
	
	bool Hit = GetWorld()->LineTraceSingleByChannel(HitRes, From, To, ECC_Visibility, TraceParams);

	if (Hit)
	{
		// DrawDebugLine(GetWorld(), From, HitRes.ImpactPoint, FColor(255, 0, 0));
		// DrawDebugBox(GetWorld(), HitRes.ImpactPoint, FVector(5, 5, 5), FColor::Emerald);
		// DrawDebugLine(GetWorld(), HitRes.ImpactPoint, To, FColor(255, 255, 0));

		return HitRes.GetActor()->GetName().Equals(Target->GetName());
	}
	return false;
}

void ABuilding::CheckForNewTarget()
{
}

void ABuilding::SetBuildingOwner(APlayerCharacter* Builder)
{
	this->BuildingOwner = Builder;
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentBuildingState)
	{
		case (EBuildingState::BS_Placing):		{ CheckBuildingLocation();					} break;
		case (EBuildingState::BS_Building):		{ Build(DeltaTime);							} break;
		case (EBuildingState::BS_Idle):			{ CheckForNewTarget();						} break;
		case (EBuildingState::BS_Attacking):	{ CheckForNewTarget(); Attack(DeltaTime);	} break;
		case (EBuildingState::BS_Disabled):		{											} break;
		default: return;
	}
}
