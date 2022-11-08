﻿// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicBuilding.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
ABasicBuilding::ABasicBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	BaseModel->SetRelativeRotation(this->GetActorRotation());
	BaseModel->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	// TODO: Fix
	TargetingArea = CreateDefaultSubobject<USphereComponent>(TEXT("TargetingCollision"));
	TargetingArea->SetupAttachment(RootComponent);
	TargetingArea->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	TargetingArea->InitSphereRadius(AttackRange);
	TargetingArea->SetRelativeLocation(GetSearchPosition());
	TargetingArea->SetVisibility(true);
	
	MaxHealthPoints = 1000;
	CurrentHealthPoints = 1;
	AttackRange = 1000;
	AttackDamage = 2;
	AttackSpeed = 0.1;
	BuildTime = 0.1;
}

FVector ABasicBuilding::GetSearchPosition()
{
	return this->GetActorLocation() + this->GetActorUpVector() * 200;
}

// Called when the game starts or when spawned
void ABasicBuilding::BeginPlay()
{
	Super::BeginPlay();

	
}

// Called every frame
void ABasicBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABasicBuilding::Build(float DeltaTime)
{
	Super::Build(DeltaTime);
}

void ABasicBuilding::Attack(float DeltaTime)
{
	Super::Attack(DeltaTime);
	
	// Attack target if we can and there is one
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (CurrentTime - LastAttackTime > AttackSpeed)
	{
		CurrentTarget->DecrementHealth(AttackDamage, BuildingOwner);
		LastAttackTime = GetWorld()->GetTimeSeconds();
	}
}

void ABasicBuilding::CheckForNewTarget()
{
	Super::CheckForNewTarget();

	AEnemy* NextTarget = nullptr;
	float ShortestDistance = AttackRange; // Set maximum distance that we can attack (collision sphere radius)

	// TODO: Implement sphere collision detection
	TArray<AActor*> Actors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Actors);
	TargetingArea->GetOverlappingActors(Actors, AEnemy::StaticClass());
	
	// Always try to attack the closet enemy
	for (AActor* Actor : Actors)
	{
		// Might be redundant check
		if (AEnemy* Enemy = Cast<AEnemy>(Actor))
		{
			float Distance = FVector::Dist(GetSearchPosition(), Enemy->GetActorLocation());
			
			if (Distance < ShortestDistance && HasLineOfSight(Enemy))
			{
				NextTarget = Enemy;
			}
		}
	}
	CurrentTarget = NextTarget;

	// Set Building state
	if (CurrentTarget != nullptr)
		CurrentBuildingState = BuildingState::Attacking;
	else
		CurrentBuildingState = BuildingState::Idle;
}

