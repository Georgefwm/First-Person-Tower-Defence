// Fill out your copyright notice in the Description page of Project Settings.

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

void ABasicBuilding::Build(float DeltaTime)
{
	Super::Build(DeltaTime);
}

void ABasicBuilding::Attack(float DeltaTime)
{
	Super::Attack(DeltaTime);

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

void ABasicBuilding::CheckForNewTarget()
{
	Super::CheckForNewTarget();

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

