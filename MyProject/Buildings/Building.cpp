// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include <string>

#include "BuildingStats.h"
#include "EngineUtils.h"
#include "Engine/StaticMeshSocket.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/Enemies/Enemy.h"
#include "Particles/ParticleSystemComponent.h"


// Sets default values
ABuilding::ABuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	
	MeshRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SharedRoot"));
	SetRootComponent(MeshRoot);

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(MeshRoot);
	float const BoxHeight = 55.0;
	CollisionComponent->InitBoxExtent(FVector(80, 80, BoxHeight));
	CollisionComponent->SetRelativeLocation(FVector(0, 0, BoxHeight + 1)); // +1 to avoid ground collision
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	AnimateYawMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("YawAnimationMesh"));
	AnimateYawMesh->SetupAttachment(MeshRoot);
	AnimateYawMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	AnimatePitchMesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PitchAnimationMesh"));
	AnimatePitchMesh->SetupAttachment(AnimateYawMesh);
	AnimatePitchMesh->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	
	ValidPlacementMaterial = CreateDefaultSubobject<UMaterial>(TEXT("ValidPlacementMaterial"));
	ValidPlacementMaterial = LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/Buildings/GlobalMaterials/ValidPreviewMaterial'"));
	
	InvalidPlacementMaterial = CreateDefaultSubobject<UMaterial>(TEXT("InvalidPlacementMaterial"));
	ValidPlacementMaterial = LoadObject<UMaterial>(nullptr, TEXT("Material'/Game/Buildings/GlobalMaterials/InvalidPreviewMaterial'"));
	
	TurretMaterial = CreateDefaultSubobject<UMaterial>(TEXT("TurretMaterial"));
	
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
	
	BuildingDataTable = LoadObject<UDataTable>(GetWorld(), TEXT("/Game/Buildings/BuildingDataTable"));
}


void ABuilding::SetYawTarget(float DesiredYaw)
{
	CurrentYawTarget.Yaw = DesiredYaw;
}

void ABuilding::SetPitchTarget(float DesiredPitch)
{
	CurrentPitchTarget.Pitch = DesiredPitch;
}

void ABuilding::UpdateRotation(float DeltaTime)
{
	
	if (CurrentBuildingState == EBuildingState::BS_Attacking) // Follow target if attacking
	{
		FRotator const LookAtYawRotation = (CurrentTarget->GetActorLocation() - AnimateYawMesh->GetComponentLocation()).Rotation();
		SetYawTarget(LookAtYawRotation.Yaw);
		
		FRotator const LookAtPitchRotation = (CurrentTarget->GetActorLocation() - AnimatePitchMesh->GetComponentLocation()).Rotation();
		SetPitchTarget(LookAtPitchRotation.Pitch);
	}
	else // Return to base animation if not attacking
	{
		FRotator BaseRotation = GetActorForwardVector().Rotation();

		double const Time = GetGameTimeSinceCreation();
		
		BaseRotation.Add(0.0, FMath::Sin(Time) * 30, 0.0);
		
		SetYawTarget(BaseRotation.Yaw);
		SetPitchTarget(BaseRotation.Pitch);
	}

	// Calculate the delta yaw rotation for this tick
	double const NewYawRotation = FMath::Clamp((CurrentYawTarget - AnimateYawMesh->GetComponentRotation()).GetNormalized().Yaw,
		-MaxRotationSpeed, MaxRotationSpeed);
	
	// Calculate the delta pitch rotation for this tick
	double const NewPitchRotation = FMath::Clamp((CurrentPitchTarget - AnimatePitchMesh->GetComponentRotation()).GetNormalized().Pitch,
		-MaxRotationSpeed/2, MaxRotationSpeed/2);
	
	AnimateYawMesh->SetWorldRotation(AnimateYawMesh->GetComponentRotation() + FRotator(0.0, NewYawRotation, 0.0));
	AnimatePitchMesh->SetWorldRotation(AnimatePitchMesh->GetComponentRotation() + FRotator(NewPitchRotation, 0.0, 0.0));
}

void ABuilding::ChangeAllMeshMaterials(EMaterialState State)
{
	TArray<UActorComponent*> Components;
	GetComponents(UStaticMeshComponent::StaticClass(), Components);

	for (UActorComponent* Component : Components)
	{
		if (UStaticMeshComponent* MeshComponent = StaticCast<UStaticMeshComponent*>(Component))
		{
			// TODO: Clean up. there must be a cleaner way to do this
			switch (State)
			{
				case (EMaterialState::MS_Normal):
					{ MeshComponent->SetMaterial(0, TurretMaterial);				} continue;
				
				case (EMaterialState::MS_Valid):
					{ MeshComponent->SetMaterial(0, ValidPlacementMaterial);		} continue;
				
				case (EMaterialState::MS_Invalid):
					{ MeshComponent->SetMaterial(0, InvalidPlacementMaterial);	} continue;
				
				default: return;
			}
		}
	}
}

FString ABuilding::GetDisplayName()
{
	return DisplayName;
}

FString ABuilding::GetDescription()
{
	return Description;
}

FVector ABuilding::GetSearchPosition()
{
	return FVector(0.0, 0.0, 0.0);
}

// Called when the game starts or when spawned
void ABuilding::BeginPlay()
{
	Super::BeginPlay();

	LastAttackTime = GetWorld()->GetTimeSeconds() - 100;

	TArray<UActorComponent*> Components;
	GetComponents(UStaticMeshComponent::StaticClass(), Components);

	for (UActorComponent* Component : Components)
	{
		if (UStaticMeshComponent* MeshComponent = StaticCast<UStaticMeshComponent*>(Component))
		{
			MeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);

			if (MeshComponent->GetName().Contains("Muzzle", ESearchCase::IgnoreCase))
			{
				Muzzles.Add(MeshComponent);
			}
		}
	}

	SetBuildingState(EBuildingState::BS_Placing);
}

bool ABuilding::IsValidBuildingLocation()
{
	if (BuildingOwner->GetCurrentGold() < Cost)
	{
		return false;
	}

	TArray<AActor*> Actors;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(BuildingOwner);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	// Get all Enemies in sphere radius
	UKismetSystemLibrary::BoxOverlapActors(GetWorld(), CollisionComponent->GetComponentLocation(),
		CollisionComponent->GetUnscaledBoxExtent(), ObjectTypes, nullptr, ActorsToIgnore, Actors);
	
	return Actors.IsEmpty();
}

void ABuilding::UpdatePreview()
{
	bool const PlacementValidity = IsValidBuildingLocation();
	
	// Prevents material update every tick when not needed
	if (LastCheckedValidity == PlacementValidity)
		return;
	
	if (PlacementValidity)
	{
		ChangeAllMeshMaterials(EMaterialState::MS_Valid);
	}
	else
	{
		ChangeAllMeshMaterials(EMaterialState::MS_Invalid);
	}

	LastCheckedValidity = PlacementValidity;
}

void ABuilding::SetBuildingState(EBuildingState State)
{
	if (State == EBuildingState::BS_Placing)
	{
		// Set material to placement
		ChangeAllMeshMaterials(EMaterialState::MS_Valid);
		
		CollisionComponent->SetCollisionProfileName(FName("OverlapAll"));
	}
	else if (CurrentBuildingState == EBuildingState::BS_Placing)
	{
		// Replace the temporary placing material with the final one
		ChangeAllMeshMaterials(EMaterialState::MS_Normal);

		CollisionComponent->SetCollisionProfileName(FName("BlockAll"));
	}
	
	CurrentBuildingState = State;
}

void ABuilding::Build(float DeltaTime)
{
	if (BuildStatus >= BuildTime)
	{
		SetBuildingState(EBuildingState::BS_Idle);
	}
	
	CurrentHealthPoints += MaxHealthPoints * (DeltaTime / BuildTime);
	BuildStatus += DeltaTime;
}

void ABuilding::Attack(float DeltaTime)
{
	
}

double ABuilding::GetTargetBarrelAngleDifference()
{
	FRotator CurrentRotation = AnimatePitchMesh->GetComponentRotation();
	FRotator PerfectRotation = (CurrentTarget->GetActorLocation() - AnimatePitchMesh->GetComponentLocation()).Rotation();
	double const Diff = PerfectRotation.GetManhattanDistance(CurrentRotation);
	
	//GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, FString::Printf(TEXT("Angle: %f"), Diff));
	
	return Diff;
}

bool ABuilding::HasLineOfSight(AEnemy* Target)
{
	FVector Location = this->GetSearchPosition();

	FVector From = Location;
	FVector To = Target->GetActorLocation();

	FHitResult HitRes;
	
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	TraceParams.bFindInitialOverlaps = true;
	TraceParams.AddIgnoredActor(BuildingOwner);
	
	bool Hit = GetWorld()->LineTraceSingleByChannel(HitRes, From, To, ECC_Visibility, TraceParams);

	if (Hit)
	{
		// DrawDebugLine(GetWorld(), From, HitRes.ImpactPoint, FColor(255, 0, 0));
		// DrawDebugLine(GetWorld(), HitRes.ImpactPoint, To, FColor(255, 255, 0));

		return HitRes.GetActor()->GetName().Equals(Target->GetName());
	}
	return false;
}

void ABuilding::CheckForNewTarget()
{
	AEnemy* NextTarget = nullptr;
	float ShortestDistance = AttackRange; // Set maximum distance that we can attack (collision sphere radius)

	TArray<AActor*> Actors;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);

	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_Pawn));

	// Get all Enemies in sphere radius
	UKismetSystemLibrary::SphereOverlapActors(GetWorld(), GetSearchPosition(), AttackRange, ObjectTypes,
		AEnemy::StaticClass(), ActorsToIgnore, Actors);

	// Always try to attack the closet enemy
	for (AActor* Actor : Actors)
	{
		// Might be redundant check
		if (AEnemy* Enemy = Cast<AEnemy>(Actor))
		{
			if (!IsValid(Enemy))
				return;

			if (Enemy->IsDead)
				continue;

			float Distance = FVector::Dist(GetSearchPosition(), Enemy->GetActorLocation());

			if (Distance < ShortestDistance && HasLineOfSight(Enemy))
			{
				NextTarget = Enemy;
				ShortestDistance = Distance;
			}
		}
	}
	CurrentTarget = NextTarget;

	// Set Building state
	if (CurrentTarget != nullptr)
		SetBuildingState(EBuildingState::BS_Attacking);
	else
		SetBuildingState(EBuildingState::BS_Idle);
}

void ABuilding::SetupStats()
{
	if (!IsValid(BuildingDataTable))
	{
		UE_LOG(LogTemp, Error, TEXT("BUILDING: Data table not set"));	
		return;
	}
	
	const TCHAR* CBuildingName = *BuildingName;
	if (FBuildingStats* OutRow = BuildingDataTable->FindRow<FBuildingStats>(CBuildingName, "WeaponStats", true))
	{
		DisplayName = OutRow->DisplayName;
		Description = OutRow->Description;
		MaxHealthPoints = OutRow->MaxHealthPoints;
		AttackRange = OutRow->AttackRange;
		AttackDamage = OutRow->AttackDamage;
		AttackSpeed = OutRow->AttackSpeed;
		BuildTime = OutRow->BuildTime;
		Cost = OutRow->Cost;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to read Building data table"));
	}
}

void ABuilding::PlayFireAnimation()
{
	if (MuzzleFlashParticleSystem == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("BUILDING: Muzzle effect not set"));
		return;
	}
	
	if (!Muzzles.IsValidIndex(CurrentMuzzleIndex))
	{
		UE_LOG(LogTemp, Error, TEXT("BUILDING: Muzzle index failure"));	
		return;
	}
	
	UStaticMeshComponent* CurrentMuzzle = Muzzles[CurrentMuzzleIndex];
	UStaticMeshSocket const* MuzzleSocket = CurrentMuzzle->GetSocketByName(FName("ParticleSpawnPoint"));
	
	UParticleSystemComponent* ParticleSystem = UGameplayStatics::SpawnEmitterAttached(MuzzleFlashParticleSystem,
		CurrentMuzzle,
		FName("ParticleSpawnPoint"),
		FVector::ZeroVector,
		FRotator::ZeroRotator,
		FVector::OneVector,
		EAttachLocation::SnapToTargetIncludingScale,
		true, // Make sure emitter loops are set to 1 in particle system
		EPSCPoolMethod::AutoRelease,
		true);
	
	// Cycle to next muzzle
	CurrentMuzzleIndex = (CurrentMuzzleIndex + 1) % Muzzles.Num();
}

void ABuilding::SetBuildingOwner(APlayerCharacter* Builder)
{
	this->BuildingOwner = Builder;
}

void ABuilding::UpdatePlacementPosition(FVector Location, FRotator Rotation)
{
	SetActorLocation(Location, false, nullptr, ETeleportType::ResetPhysics);
	SetActorRotation(Rotation, ETeleportType::ResetPhysics);
}

// Called every frame
void ABuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	switch (CurrentBuildingState)
	{
		case (EBuildingState::BS_Placing):		{ UpdatePreview();							} return;
		case (EBuildingState::BS_Building):		{ Build(DeltaTime);							} return;
		case (EBuildingState::BS_Idle):			{ CheckForNewTarget();						} break;
		case (EBuildingState::BS_Attacking):	{ CheckForNewTarget(); Attack(DeltaTime);	} break;
		case (EBuildingState::BS_Disabled):		{											} break;
		default: return;
	}

	if (Rotates)
		UpdateRotation(DeltaTime);
}

void ABuilding::DestroyBuilding()
{
	Destroy();
}
