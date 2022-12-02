// Fill out your copyright notice in the Description page of Project Settings.

#include "SlowBuilding.h"
#include "Kismet/GameplayStatics.h"
#include "NiagaraFunctionLibrary.h"
#include "MyProject/ModifierEffects/SubTypes/MoveSlowModifier.h"

// Sets default values
ASlowBuilding::ASlowBuilding()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	BuildingName = FString("SlowBuilding");

	MuzzleAnimates = false;
	PitchRotation = false;

	HitBox = CreateDefaultSubobject<UBoxComponent>(TEXT("HitBox"));
	float const BoxLength = AttackRange * 2;
	HitBox->SetAbsolute(false, false, true);
	HitBox->InitBoxExtent(FVector(BoxLength, 80, 80));
	HitBox->SetupAttachment(AnimateYawMesh);
	HitBox->SetRelativeLocation(FVector(BoxLength / 2, 0, 40));
	HitBox->SetWorldScale3D(FVector::OneVector);
	HitBox->SetCollisionEnabled(ECollisionEnabled::QueryOnly);

	// Import stats from building data table
	Super::SetupStats();
}

FVector ASlowBuilding::GetSearchPosition()
{
	return GetActorLocation() + GetActorUpVector() * 100;
}

// Called when the game starts or when spawned
void ASlowBuilding::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ASlowBuilding::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ASlowBuilding::Build(float DeltaTime)
{
	Super::Build(DeltaTime);
}

void ASlowBuilding::Attack(float DeltaTime)
{
	Super::Attack(DeltaTime);
	
	// Attack target if we can and there is one
	float CurrentTime = GetWorld()->GetTimeSeconds();
	
	if (CurrentTime - LastAttackTime > AttackSpeed)
	{
		if (!IsValid(CurrentTarget))
		{
			CurrentTarget = nullptr;
			SetBuildingState(EBuildingState::BS_Idle);
			return;
		}
		
		if(GetTargetBarrelAngleDifference() > MaxAttackAngleDeviation)
			return;
		
		
	
		if (!Muzzles.IsValidIndex(CurrentMuzzleIndex))
		{
			UE_LOG(LogTemp, Error, TEXT("BUILDING: Muzzle index failure"));	
			return;
		}
	
		UStaticMeshComponent* CurrentMuzzle = Muzzles[CurrentMuzzleIndex];
		UStaticMeshSocket const* MuzzleSocket = CurrentMuzzle->GetSocketByName(FName("ParticleSpawnPoint"));

		if (FireSound)
			UGameplayStatics::PlaySoundAtLocation(GetWorld(), FireSound, CurrentMuzzle->GetComponentLocation(),
				FRotator::ZeroRotator, 0.2, 1, 0);

		
		if (IceEffectNiagaraSystem)
		{
			FVector EffectSpawnLocation = CurrentMuzzle->GetComponentLocation();
			EffectSpawnLocation.Z = 1.5;
		
			UNiagaraFunctionLibrary::SpawnSystemAtLocation(GetWorld(), IceEffectNiagaraSystem,
				EffectSpawnLocation,
				CurrentMuzzle->GetComponentRotation(),
				FVector::OneVector,
				true,
				true,
				ENCPoolMethod::AutoRelease,
				true);
		}
		
		TArray<AActor*> OverlappingActors;

		HitBox->GetOverlappingActors(OverlappingActors, AEnemy::StaticClass());

		for (AActor* Actor : OverlappingActors)
		{
			if (AEnemy* Enemy = Cast<AEnemy>(Actor))
			{
				CurrentTarget->LastDamageDealer = BuildingOwner;
				CurrentTarget->DecrementHealth(AttackDamage);
				CurrentTarget->GiveModifier(AMoveSlowModifier::StaticClass());
			}
		}
		
		LastAttackTime = GetWorld()->GetTimeSeconds();
	}
}

void ASlowBuilding::CheckForNewTarget()
{
	Super::CheckForNewTarget();
}

