// Fill out your copyright notice in the Description page of Project Settings.

#include "SlowBuilding.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/ModifierEffects/SubTypes/MoveSlowModifier.h"

// Sets default values
ASlowBuilding::ASlowBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	
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

	AEnemy* NextTarget = nullptr;
	float ShortestDistance = AttackRange; // Set maximum distance that we can attack (collision sphere radius)
	
	TArray<AActor*> Actors;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// Get all Enemies in sphere radius
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetSearchPosition(), AttackRange, ObjectTypes,
		AEnemy::StaticClass(), ActorsToIgnore, Actors);
	
	// Always try to attack the closet enemy
	for (AActor* Actor : Actors)
	{
		// Might be redundant check
		if (AEnemy* Enemy = Cast<AEnemy>(Actor))
		{
			if (!IsValid(Enemy))
				return;
			
			float Distance = FVector::Dist(GetSearchPosition(), Enemy->GetActorLocation());
			
			if (Distance < ShortestDistance && HasLineOfSight(Enemy))
			{
				NextTarget = Enemy;
				ShortestDistance = Distance;
			}
		}
	}
	CurrentTarget = NextTarget;

	// Set Building state
	if (CurrentTarget != nullptr)
		SetBuildingState(EBuildingState::BS_Attacking);
	else
		SetBuildingState(EBuildingState::BS_Idle);
}

