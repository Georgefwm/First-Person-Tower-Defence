// Fill out your copyright notice in the Description page of Project Settings.


#include "HealthPriority.h"

UHealthPriority::UHealthPriority()
{
	Name = "Highest HP";
}

AEnemy* UHealthPriority::Compare(ABuilding *Asker, TArray<AActor*> &Candidates)
{
	AEnemy* BestTarget = nullptr;
	int HighestHealth = 0;

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

			int CandidateHealth = Enemy->GetHP();

			if (CandidateHealth > HighestHealth)
			{
				BestTarget = Enemy;
				HighestHealth = CandidateHealth;
			}
		}
	}
	
	return BestTarget;
}
