// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "CookerSettings.h"
#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthPoints = MaxHealthPoints;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::HandleHit(FHitResult Hit, int Damage)
{
	int FinalDamage = Damage;
	
	const FString BoneName = Hit.BoneName.ToString();
	if (BoneName == FString("Bip001-Head"))
	{
		FinalDamage *= 3;
		// TODO: Play distinct headshot sound
	}
	
	DecrementHealth(FinalDamage);
}

// Handles healing and damage
void AEnemy::DecrementHealth(int Damage)
{
	if (HealthPoints - Damage <= 0)
	{
		Destroy();
	}

	HealthPoints -= Damage;

	OnHPUpdate();
}

// Handles healing and damage
void AEnemy::IncrementHealth(int Healing)
{
	if (HealthPoints + Healing > MaxHealthPoints)
	{
		HealthPoints = MaxHealthPoints;
	}

	HealthPoints += Healing;

	OnHPUpdate();
}

