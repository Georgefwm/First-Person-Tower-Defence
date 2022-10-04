// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyProject/Enemies/Enemy.h"


// Sets default values
ABuilding::ABuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

}

FVector ABuilding::GetSearchPosition()
{
	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Warning: ABuilding::GetSearchPosition() virtual function should never be called"));
	return FVector(0, 0, 0);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();
}

void ABuilding::Build(float DeltaTime)
{
	if (this->BuildStatus >= this->BuildTime)
		this->FinishedBuilding = true;
	
	CurrentHealthPoints += this->MaxHealthPoints * (DeltaTime / this->BuildTime);
	this->BuildStatus += DeltaTime;
}

void ABuilding::Attack(float DeltaTime)
{
	
}

bool ABuilding::HasLineOfSight()
{
	FVector Location = this->GetSearchPosition();

	FVector From = Location;
	FVector To = this->CurrentTarget->GetActorLocation();

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

		return HitRes.GetActor()->GetName().Equals(CurrentTarget->GetName());
	}
	return false;
}

void ABuilding::CheckForNewTarget()
{
	
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (FinishedBuilding)
		Attack(DeltaTime);
	else
		Build(DeltaTime);
}

