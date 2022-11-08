// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
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

	// Adjust the targeting function based on target priority
	enum TargetPriority : uint8
	{
		LowestDistance,
		HighestDistance,
		HighestHp,
		LowestHp,
	};

	enum BuildingState : uint8
	{
		Building,
		Idle,
		Attacking,
		Disabled
	};

	/** Stats */
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	int MaxHealthPoints;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float AttackRange;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	int AttackDamage;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float AttackSpeed;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float BuildTime;

	
	/** Runtime vars */

	// Holds reference to the player who built the building
	UPROPERTY()
	APlayerCharacter* BuildingOwner;

	// Tracks HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	int CurrentHealthPoints = 1;

	// Tracks progress of build state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float BuildStatus = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Properties")
	AEnemy* CurrentTarget = nullptr;

	// Decide how the building should target available enemies
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Properties")
	uint8 CurrentTargetPriority = TargetPriority::LowestDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Properties")
	uint8 CurrentBuildingState = BuildingState::Building;

	UFUNCTION(BlueprintCallable)
	uint8 GetCurrentBuildingState() { return CurrentBuildingState; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float LastAttackTime;
	
	// Produces references for target candidates
	UPROPERTY()
	USphereComponent* TargetingArea;

	// Returns the position that the building attacks from
	UFUNCTION()
	virtual FVector GetSearchPosition();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	virtual void Build(float DeltaTime);

	UFUNCTION()
	virtual void Attack(float DeltaTime);

	UFUNCTION()
	bool HasLineOfSight(AEnemy* Target);

	UFUNCTION()
	virtual void CheckForNewTarget();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION()
	void UpdateTargeting();

	UFUNCTION()
	void SetBuildingOwner(APlayerCharacter* Builder);

	// TODO: Implement better target priority system (closest, furthest, highest hp, etc)
};
