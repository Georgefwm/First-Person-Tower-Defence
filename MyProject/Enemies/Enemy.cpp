// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Blueprint/UserWidget.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	this->HealthPoints = this->MaxHealthPoints;
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

void AEnemy::ApplyDamage(int Damage)
{
	if(IsValid(this))
	{
		if (this->HealthPoints - Damage <= 0)
		{
			this->Destroy();
		}
		this->HealthPoints -= Damage;

		// Trigger event for hp bar
		OnHPUpdate();
	}
}

