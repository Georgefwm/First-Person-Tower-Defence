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
	CheckForNewTarget();

	// Attack target if we can and there is one
	if (this->AttackCooldown <= 0 && this->CurrentTarget)
	{
		

		// GEngine->AddOnScreenDebugMessage(-1, 1.0f, FColor::Red, HitRes.GetActor()->GetName());
		
		// Only deal damage if there is nothing in the way
		if (this->HasLineOfSight())
		{
			this->CurrentTarget->ApplyDamage(this->AttackDamage);
			this->AttackCooldown = this->AttackSpeed;
		}
	}
	else
	{
		// stop bes things happening if tower doesnt attack for a looooong time
		if (this->AttackCooldown <= 0) AttackCooldown = -1;
		else this->AttackCooldown -= DeltaTime;
	}
}

bool ABuilding::HasLineOfSight()
{
	FVector Location = this->GetSearchPosition();

	FVector From = Location;
	FVector To = this->CurrentTarget->GetActorLocation();

	FHitResult HitRes;

	FCollisionQueryParams TraceParms;
	TraceParms.AddIgnoredActor(this);
	TraceParms.bFindInitialOverlaps = true;
	
	bool Hit = GetWorld()->LineTraceSingleByChannel(HitRes, To, From, ECC_Visibility, TraceParms);
	DrawDebugLine(GetWorld(), From, To, FColor(255, 0, 0));

	if (Hit)
	{
		DrawDebugLine(GetWorld(), From, HitRes.ImpactPoint, FColor(255, 0, 0));
		DrawDebugBox(GetWorld(), HitRes.ImpactPoint, FVector(5, 5, 5), FColor::Emerald);
		DrawDebugLine(GetWorld(), HitRes.ImpactPoint, To, FColor(255, 255, 0));
		
		// GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, HitRes.GetActor()->GetActorNameOrLabel());
		return true;
	}
	return true;
}

void ABuilding::CheckForNewTarget()
{
	AEnemy* NextTarget = nullptr;
	float ShortestDistance = static_cast<float>(this->AttackRange);
	
	if (IsValid(this->CurrentTarget))
	{
		NextTarget = this->CurrentTarget;
		ShortestDistance = FVector::Dist(this->GetActorLocation(), this->CurrentTarget->GetActorLocation());
		// DrawDebugLine(GetWorld(), this->GetSearchPosition(), CurrentTarget->GetActorLocation(), FColor(2, 2, 2));
	}
	
	TArray<AActor*> Actors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Actors);
	// Always try to attack the closet enemy
	for (AActor* e : Actors)
	{
		AEnemy* Enemy = Cast<AEnemy>(e);
		if (Enemy)
		{
			if (FVector::Dist(this->GetActorLocation() + this->GetActorUpVector() * 200, Enemy->GetActorLocation()) < ShortestDistance)
			{
				NextTarget = Enemy;
			}
		}
	}

	this->CurrentTarget = NextTarget;
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

