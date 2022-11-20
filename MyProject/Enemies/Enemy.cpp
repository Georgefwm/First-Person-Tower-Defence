// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "GameFramework/GameModeBase.h"
#include "GameFramework/GameStateBase.h"
#include "Kismet/KismetMathLibrary.h"
#include "MyProject/Core/TowerDefenceGameMode.h"
#include "MyProject/Core/TowerDefenceGameState.h"
#include "MyProject/ModifierEffects/ModiferComponent.h"
#include "MyProject/ModifierEffects/SubTypes/MoveSlowModifier.h"

// Sets default values
AEnemy::AEnemy()
{
	ModifierComponent = CreateDefaultSubobject<UModiferComponent>(TEXT("Modifier Component"));
	
	GetMesh()->SetCollisionProfileName(TEXT("OverlapAllDynamic"));
	
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	HealthPoints = MaxHealthPoints;
	
	static ConstructorHelpers::FClassFinder<AActor> ClassFinder(TEXT("/Game/Player/CombatText/HeadshotTextBP"));
	HeadshotTextClass = ClassFinder.Class;
}

// Called when the game starts or when spawned
void AEnemy::BeginPlay()
{
	Super::BeginPlay();
	GetMesh()->SetCollisionResponseToChannel(ECollisionChannel::ECC_Visibility, ECollisionResponse::ECR_Block);

	
	
	GetCharacterMovement()->MaxWalkSpeed = MoveSpeed;
}

// Called every frame
void AEnemy::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AEnemy::HandleHit(FHitResult Hit, int Damage, APlayerCharacter* Shooter)
{
	if (IsDead)
	{
		return;
	}
	
	int FinalDamage = Damage;
	
	
	
	const FString BoneName = Hit.BoneName.ToString();
	if (BoneName == FString("Bip001-Head"))
	{
		FinalDamage *= 2;

		FVector Location = GetActorLocation() +
			GetActorUpVector() * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		
		GetWorld()->SpawnActor<AHeadshotText>(HeadshotTextClass, Location, GetActorRotation());
		// TODO: Play distinct headshot sound
	}
	
	LastDamageDealer = Shooter;
	DecrementHealth(FinalDamage);
}

// Handles healing and damage
void AEnemy::DecrementHealth(int Damage)
{
	if (HealthPoints - Damage <= 0)
	{
		if (!IsDead)
		{
			IsDead = true;

			// Dead now, so no modifiers should be applied
			ModifierComponent->SetModifierImmune(true);

			Die();
		}
	}

	HealthPoints -= Damage;

	// Redundant?
	OnHPUpdate();
}

void AEnemy::SetMoveSpeed(float Speed)
{
	this->GetCharacterMovement()->MaxWalkSpeed = Speed;
}

void AEnemy::GetModifiers(TArray<AModifier*> Mods)
{
	TArray<AActor*> AttachedActors;
	
	GetAttachedActors(AttachedActors);

	for (AActor* Actor : AttachedActors)
	{
		if (AModifier* Modifer = Cast<AModifier>(Actor))
		{
			Mods.Add(Modifer);
		}
	}
}

void AEnemy::GiveModifier(UClass* ModClass)
{
	
	ModifierComponent->ApplyModifier(ModClass);
}

void AEnemy::Die()
{
	if (LastDamageDealer != nullptr)
		LastDamageDealer->IncrementGold(GoldValue);

	ModifierComponent->RemoveAllModifiers();

	USkeletalMeshComponent* ModelMesh = GetMesh();
	UCapsuleComponent* Capsule = GetCapsuleComponent();

	Capsule->SetCollisionProfileName(TEXT("Ragdoll"));
	ModelMesh->SetCollisionProfileName(TEXT("Ragdoll"));

	Capsule->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);
	ModelMesh->SetCollisionResponseToChannel(ECollisionChannel::ECC_WorldDynamic, ECollisionResponse::ECR_Ignore);

	ModelMesh->SetSimulatePhysics(true);
	ModelMesh->AddImpulse(GetVelocity());

	// Any things that need updating in BP. e.g. Health bar widget removal
	OnDieBp();
	
	GetCharacterMovement()->DisableMovement();

	SetLifeSpan(2);

	// Tell the game to check if the wave has finished
	AGameStateBase* GameState = GetWorld()->GetGameState();
	if (ATowerDefenceGameState* TDGameState = StaticCast<ATowerDefenceGameState*>(GameState))
	{
		TDGameState->UpdateGameState();
	}
}

void AEnemy::ModifyMaxHealth(int NewHealth)
{
	if (NewHealth <= 1)
		return;
	
	MaxHealthPoints = NewHealth;
	HealthPoints = MaxHealthPoints;
}

// Handles healing and damage
void AEnemy::IncrementHealth(int Healing)
{
	if (HealthPoints + Healing > MaxHealthPoints)
	{
		HealthPoints = MaxHealthPoints;
	}

	HealthPoints += Healing;

	
	OnHPUpdate();
}

