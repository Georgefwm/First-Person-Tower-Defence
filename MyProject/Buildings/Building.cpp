// Fill out your copyright notice in the Description page of Project Settings.


#include "Building.h"

#include "BuildingStats.h"
#include "EngineUtils.h"
#include "Kismet/GameplayStatics.h"
#include "MyProject/Enemies/Enemy.h"


// Sets default values
ABuilding::ABuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	TurretBaseMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretBase"));
	RootComponent = TurretBaseMeshComponent;
	TurretBaseMeshComponent->SetRelativeScale3D(FVector(20.0, 20.0, 20.0));
	

	CollisionComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("CollisionComponent"));
	CollisionComponent->SetupAttachment(TurretBaseMeshComponent);
	float const BoxHeight = 55;
	CollisionComponent->SetRelativeScale3D(FVector(1/20.0, 1/20.0, 1/20.0));
	CollisionComponent->InitBoxExtent(FVector(80, 80, BoxHeight));
	CollisionComponent->SetRelativeLocation(FVector(0, 0, 2.75));
	CollisionComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	
	TurretGunMeshComponent = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("TurretGun"));
	TurretGunMeshComponent->SetupAttachment(TurretBaseMeshComponent);
	TurretGunMeshComponent->SetRelativeScale3D(FVector(1.0, 1.0, 1.0));

	
	ValidPlacementMaterial = CreateDefaultSubobject<UMaterial>(TEXT("ValidPlacementMaterial"));
	InvalidPlacementMaterial = CreateDefaultSubobject<UMaterial>(TEXT("InvalidPlacementMaterial"));
	
	TurretBaseMaterial = CreateDefaultSubobject<UMaterial>(TEXT("BaseMat"));
	TurretGunMaterial = CreateDefaultSubobject<UMaterial>(TEXT("GunMat"));
	
	SpawnCollisionHandlingMethod = ESpawnActorCollisionHandlingMethod::AlwaysSpawn;
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

	SetBuildingState(EBuildingState::BS_Placing);
}

bool ABuilding::IsValidBuildingLocation()
{
	TArray<AActor*> Actors;

	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(this);
	ActorsToIgnore.Add(BuildingOwner);
	
	TArray<TEnumAsByte<EObjectTypeQuery>> ObjectTypes;
	ObjectTypes.Add(UEngineTypes::ConvertToObjectType(ECollisionChannel::ECC_WorldStatic));

	// Get all Enemies in sphere radius
	UKismetSystemLibrary::BoxOverlapActors(GetWorld(), GetSearchPosition(), CollisionComponent->GetUnscaledBoxExtent(),
		ObjectTypes, nullptr, ActorsToIgnore, Actors);
	

	return Actors.IsEmpty();
}

void ABuilding::UpdatePreview()
{
	
	if (IsValidBuildingLocation())
	{
		TurretBaseMeshComponent->SetMaterial(0, ValidPlacementMaterial);
		TurretGunMeshComponent->SetMaterial(0, ValidPlacementMaterial);
	}
	else
	{
		TurretBaseMeshComponent->SetMaterial(0, InvalidPlacementMaterial);
		TurretGunMeshComponent->SetMaterial(0, InvalidPlacementMaterial);
	}
	
}

void ABuilding::SetBuildingState(EBuildingState State)
{
	if (State == EBuildingState::BS_Placing)
	{
		TurretBaseMeshComponent->SetMaterial(0, ValidPlacementMaterial);
		TurretGunMeshComponent->SetMaterial(0, ValidPlacementMaterial);

		TurretBaseMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
		TurretGunMeshComponent->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	}
	else if (State != EBuildingState::BS_Placing && CurrentBuildingState == EBuildingState::BS_Placing)
	{
		TurretBaseMeshComponent->SetMaterial(0, TurretBaseMaterial);
		TurretGunMeshComponent->SetMaterial(0, TurretGunMaterial);

		TurretBaseMeshComponent->SetCollisionProfileName(FName("BlockAllDynamic"));
		TurretGunMeshComponent->SetCollisionProfileName(FName("BlockAllDynamic"));
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
	if (BuildingDataTable == nullptr)
	{
		UE_LOG(LogTemp, Error, TEXT("Data table not set for weapon"));	
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
		case (EBuildingState::BS_Placing):		{ UpdatePreview();							} break;
		case (EBuildingState::BS_Building):		{ Build(DeltaTime);							} break;
		case (EBuildingState::BS_Idle):			{ CheckForNewTarget();						} break;
		case (EBuildingState::BS_Attacking):	{ CheckForNewTarget(); Attack(DeltaTime);	} break;
		case (EBuildingState::BS_Disabled):		{											} break;
		default: return;
	}

	if (CurrentBuildingState == EBuildingState::BS_Placing || CurrentBuildingState == EBuildingState::BS_Building)
		return;
	
	FRotator NewRotation = GetActorRotation();
	if (CurrentBuildingState == EBuildingState::BS_Attacking)
	{
		FRotator const LookAtRotation = (CurrentTarget->GetActorLocation() - TurretGunMeshComponent->GetComponentLocation()).Rotation();
		NewRotation.Yaw = LookAtRotation.Yaw;
	}
	NewRotation.Yaw -= 90;
	TurretGunMeshComponent->SetWorldRotation(NewRotation);
}

void ABuilding::DestroyBuilding()
{
	Destroy();
}
