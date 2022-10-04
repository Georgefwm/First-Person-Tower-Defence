// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "MyProject/Enemies/Enemy.h"
#include "Building.generated.h"

UCLASS()
class MYPROJECT_API ABuilding : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuilding();

	enum TargetPriority : uint8
	{
		TP_DIST_LOWEST,
		TP_DIST_HIGHEST,
		TP_HP_LOWEST,
		TP_HP_HIGHEST,
	};
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	int MaxHealthPoints = 1000;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	int CurrentHealthPoints = 1;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	double AttackRange = 10000;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	double AttackDamage = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	double AttackSpeed = 1;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float AttackCooldown = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float BuildTime = 5;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float BuildStatus = 0;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	bool FinishedBuilding = false;

	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Properties")
	AEnemy* CurrentTarget = nullptr;

	UFUNCTION()
	virtual FVector GetSearchPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Build(float DeltaTime);

	UFUNCTION()
	virtual void Attack(float DeltaTime);

	bool HasLineOfSight();

	void CheckForNewTarget();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// TODO: Implement better target priority system (closest, furthest, highest hp, etc)
};
