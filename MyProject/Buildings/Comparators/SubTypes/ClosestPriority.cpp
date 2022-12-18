// Fill out your copyright notice in the Description page of Project Settings.


#include "ClosestPriority.h"

UClosestPriority::UClosestPriority()
{
	Name = "Closest";
}

AEnemy* UClosestPriority::Compare(ABuilding *Asker, TArray<AActor*> &Candidates)
{
	AEnemy* BestTarget = nullptr;
	float ShortestDistance = 10000;
	
	// Always try to attack the closet enemy
	for (AActor* Actor : Candidates)
	{
		// Might be redundant check
		if (AEnemy* Enemy = Cast<AEnemy>(Actor))
		{
			if (!IsValid(Enemy))
				continue;

			if (Enemy->IsDead)
				continue;

			float Distance = FVector::Dist(Asker->GetSearchPosition(), Enemy->GetActorLocation());

			if (Distance < ShortestDistance && Asker->HasLineOfSight(Enemy))
			{
				BestTarget = Enemy;
				ShortestDistance = Distance;
			}
		}
	}
	
	return BestTarget;
}
