// Fill out your copyright notice in the Description page of Project Settings.


#include "EnemySpawn.h"


// Sets default values
AEnemySpawn::AEnemySpawn()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	SpawnRate = 0.5;
}

void AEnemySpawn::GenerateWave(int WaveNumber)
{
	if (EnemyClasses.IsEmpty())
	{
		UE_LOG(LogTemp, Error, TEXT("WAVESPAWNER: No enemies enabled, cannot generate wave"));
		return;
	}
	
	WaveSpawnOrder.Empty();
	
	int WaveSize = WaveNumber * WaveNumber + 1;

	for (int EnemyCount = 0; EnemyCount < WaveSize; EnemyCount++)
	{
		// TODO: Make more flexible for more enemy types
		WaveSpawnOrder.Add(EnemyClasses[0]);
	}
}

void AEnemySpawn::StartWave()
{
	GetWorldTimerManager().SetTimer(SpawnTimer, this, &AEnemySpawn::SpawnEnemy, SpawnRate, true, 1.0);
}

void AEnemySpawn::SpawnEnemy()
{
	UE_LOG(LogTemp, Warning, TEXT("SPAWN: Spawning enemy"));
	if (WaveSpawnOrder.IsEmpty())
	{
		GetWorldTimerManager().ClearTimer(SpawnTimer);
		return;
	}

	FVector Location = GetActorLocation();
	FRotator Rotation = GetActorRotation();

	FActorSpawnParameters Params;
	Params.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButAlwaysSpawn;
	
	AEnemy* NewEnemy = GetWorld()->SpawnActor<AEnemy>(WaveSpawnOrder.Pop(), Location, Rotation, Params);
}

bool AEnemySpawn::WaveSpawnsRemaining()
{
	return !WaveSpawnOrder.IsEmpty();
}

// Called when the game starts or when spawned
void AEnemySpawn::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void AEnemySpawn::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

