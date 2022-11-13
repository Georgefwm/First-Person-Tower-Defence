// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "MyProject/Enemies/Enemy.h"
#include "Building.generated.h"

UENUM()
enum class ETargetPriority : uint8
{
	TP_LowestDistance		UMETA(DisplayName="Lowest Distance"),
	TP_HighestDistance		UMETA(DisplayName="Highest Distance"),
	TP_HighestHp			UMETA(DisplayName="Highest HP"),
	TP_LowestHp				UMETA(DisplayName="Lowest HP"),
};

UENUM()
enum class EBuildingState : uint8
{
	BS_Placing		UMETA(DisplayName="Placing"),
	BS_Building		UMETA(DisplayName="Building"),
	BS_Idle			UMETA(DisplayName="Idle"),
	BS_Attacking	UMETA(DisplayName="Attacking"),
	BS_Disabled		UMETA(DisplayName="Disabled")
};


UCLASS()
class MYPROJECT_API ABuilding : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuilding();

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

	UPROPERTY()
	USkeletalMesh* Mesh;

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
	ETargetPriority CurrentTargetPriority = ETargetPriority::TP_LowestDistance;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Properties")
	EBuildingState CurrentBuildingState = EBuildingState::BS_Building;

	UFUNCTION(BlueprintCallable)
	EBuildingState GetCurrentBuildingState() { return CurrentBuildingState; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float LastAttackTime;

	// Returns the position that the building attacks from
	UFUNCTION()
	virtual FVector GetSearchPosition();

	UFUNCTION()
	void SetBuildingState(EBuildingState State);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
	void CheckBuildingLocation();

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
	void SetBuildingOwner(APlayerCharacter* Builder);

	// TODO: Implement better target priority system (closest, furthest, highest hp, etc)
};
