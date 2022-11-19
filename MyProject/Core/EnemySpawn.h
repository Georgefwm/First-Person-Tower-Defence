// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProject/Enemies/Enemy.h"
#include "EnemySpawn.generated.h"

UCLASS()
class MYPROJECT_API AEnemySpawn : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AEnemySpawn();

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<AEnemy>> EnemyClasses;

	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AEnemy>> WaveSpawnOrder;

	UFUNCTION()
	void GenerateWave(int WaveNumber);

	UFUNCTION()
	void StartWave();

	UFUNCTION()
	void SpawnEnemy();

	UFUNCTION()
	bool WaveSpawnsRemaining();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	FTimerHandle SpawnTimer;

	// How fast enemies get spawned
	UPROPERTY()
	float SpawnRate;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
