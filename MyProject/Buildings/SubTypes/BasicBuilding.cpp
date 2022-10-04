// Fill out your copyright notice in the Description page of Project Settings.

#include "BasicBuilding.h"

// Sets default values
ABasicBuilding::ABasicBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	BaseModel = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BarrelMesh"));
	BaseModel->SetRelativeRotation(this->GetActorRotation());
	BaseModel->SetRelativeLocation(FVector(0.0f, 0.0f, 0.0f));

	MaxHealthPoints = 1000;
	CurrentHealthPoints = 1;
	AttackRange = 1000;
	AttackDamage = 2;
	AttackSpeed = 0.1;
	BuildTime = 2;
	
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

