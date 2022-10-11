// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Blueprint/UserWidget.h"
#include "Net/UnrealNetwork.h"

// Sets default values
AEnemy::AEnemy()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthPoints = MaxHealthPoints;

	bReplicates = true;
}

void AEnemy::GetLifetimeReplicatedProps(TArray< FLifetimeProperty >& OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);
	
	DOREPLIFETIME(AEnemy, HealthPoints);
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

	if (HealthPoints - Damage <= 0)
	{
		Destroy();
	}
	HealthPoints -= Damage;

	// Trigger event for hp bar
	OnHPUpdate();
}

