// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/BoxComponent.h"
#include "GameFramework/Actor.h"
#include "MyProject/Enemies/Enemy.h"
#include "Building.generated.h"

class UDataTable;

UENUM()
enum class ETargetPriority : uint8
{
	TP_LowestDistance		UMETA(DisplayName="Lowest Distance"),
	TP_HighestDistance		UMETA(DisplayName="Highest Distance"),
	TP_HighestHp			UMETA(DisplayName="Highest HP"),
	TP_LowestHp				UMETA(DisplayName="Lowest HP")
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

UENUM()
enum class EMaterialState : uint8
{
	MS_Valid		UMETA(DisplayName="Valid"),
	MS_Invalid		UMETA(DisplayName="Invalid"),
	MS_Normal		UMETA(DisplayName="Normal")
};


UCLASS()
class MYPROJECT_API ABuilding : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuilding();

	/** Stats */

	UPROPERTY(VisibleAnywhere, Category = "Building Properties")
	UDataTable* BuildingDataTable;

	UPROPERTY()
	FString BuildingName;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	int MaxHealthPoints;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float AttackRange;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	int AttackDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float AttackSpeed;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float BuildTime;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	int Cost;
	
	UPROPERTY(VisibleAnywhere, Category = "Building Properties")
	bool Rotates = true;

	
	/** Runtime vars */

	// Holds reference to the player who built the building
	UPROPERTY()
	APlayerCharacter* BuildingOwner;

	UPROPERTY(EditDefaultsOnly)
	UBoxComponent* CollisionComponent;

	// Maximum angle deviation allowed to attack enemy
	UPROPERTY()
	double MaxAttackAngleDeviation = 5.0;
	
	// Speed of rotation interpolation in degrees
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float MaxRotationSpeed = 2.0;

	/** Mesh */

	UPROPERTY(EditDefaultsOnly)
	USceneComponent* MeshRoot;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* AnimateYawMesh;

	UPROPERTY(EditDefaultsOnly)
	UStaticMeshComponent* AnimatePitchMesh;

	/** Mesh 'Animation' */

	UPROPERTY()
	TArray<UStaticMeshComponent*> Muzzles;

	UPROPERTY()
	int CurrentMuzzleIndex = 0;

	UPROPERTY(EditAnywhere, Category = "Animation")
	UParticleSystem* MuzzleFlashParticleSystem;

	UPROPERTY()
	FRotator CurrentYawTarget = FRotator(0.0, 0.0, 0.0);

	UPROPERTY()
	FRotator CurrentPitchTarget = FRotator(0.0, 0.0, 0.0);

	UFUNCTION()
	void SetYawTarget(float DesiredYaw);

	UFUNCTION()
	void SetPitchTarget(float DesiredPitch);

	UFUNCTION()
	void UpdateRotation(float DeltaTime);

	/** Material */

	UPROPERTY()
	bool LastCheckedValidity = false;
	
	UPROPERTY()
	UMaterialInterface* ValidPlacementMaterial;

	UPROPERTY()
	UMaterialInterface* InvalidPlacementMaterial;

	UPROPERTY(EditDefaultsOnly, Category = "Material")
	UMaterialInterface* TurretMaterial;

	UFUNCTION()
	void ChangeAllMeshMaterials(EMaterialState State);

	/** Sounds */
	
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Sound")
	USoundBase* FireSound;
	

	// Tracks HP
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	int CurrentHealthPoints = 1;

	// Tracks progress of build state
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float BuildStatus = 0;
	
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Properties")
	AEnemy* CurrentTarget = nullptr;

	/** UI properties */
	
	UPROPERTY()
	FString DisplayName;

	UFUNCTION(BlueprintCallable)
	FString GetDisplayName();
	
	UPROPERTY()
	FString Description;

	UFUNCTION(BlueprintCallable)
	FString GetDescription();

	
	// Decide how the building should target available enemies
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Properties")
	ETargetPriority CurrentTargetPriority = ETargetPriority::TP_LowestDistance;

	// Dictates tick behaviour
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = "Building Properties")
	EBuildingState CurrentBuildingState = EBuildingState::BS_Building;

	UFUNCTION(BlueprintCallable)
	EBuildingState GetCurrentBuildingState() { return CurrentBuildingState; }

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Building Properties")
	float LastAttackTime;

	// Returns the position that the building attacks from
	UFUNCTION()
	virtual FVector GetSearchPosition();

	// Sets building state, handles collision profile and material changes based on state
	UFUNCTION()
	void SetBuildingState(EBuildingState State);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	// Updates materials based on IsValidBuildingLocation function
	UFUNCTION()
	void UpdatePreview();

	// Progress build state or exit if complete
	UFUNCTION()
	virtual void Build(float DeltaTime);

	// Apply damage and modifiers to current target
	UFUNCTION()
	virtual void Attack(float DeltaTime);

	// Calculates the difference in angle between perfect line to target and turrets current line to target
	UFUNCTION()
	double GetTargetBarrelAngleDifference();

	// Checks if the turret has unobstructed line of sight to (potential) target
	UFUNCTION()
	bool HasLineOfSight(AEnemy* Target);

	// Finds best eligible target
	UFUNCTION()
	virtual void CheckForNewTarget();

	// Load stats from the building data table
	UFUNCTION()
	virtual void SetupStats();

	UFUNCTION()
	virtual void PlayFireAnimation();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Checks if current location is valid position 
	UFUNCTION()
	bool IsValidBuildingLocation();

	// Used by BuildTool to move the building around (for finding a valid position)
	void UpdatePlacementPosition(FVector Location, FRotator Rotation);

	// Handles cleanup logic and then destroys itself
	UFUNCTION()
	void DestroyBuilding();

	// Used to set the building owner reference
	UFUNCTION()
	void SetBuildingOwner(APlayerCharacter* Builder);

	// TODO: Implement better target priority system (closest, furthest, highest hp, etc)
	
};
