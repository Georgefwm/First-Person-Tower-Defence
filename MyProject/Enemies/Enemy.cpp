// Fill out your copyright notice in the Description page of Project Settings.


#include "Enemy.h"

#include "Blueprint/UserWidget.h"
#include "Components/CapsuleComponent.h"
#include "GameFramework/CharacterMovementComponent.h"
#include "MyProject/ModifierEffects/ModiferComponent.h"
#include "MyProject/ModifierEffects/SubTypes/MoveSlowModifier.h"

// Sets default values
AEnemy::AEnemy()
{
	ModifierComponent = CreateDefaultSubobject<UModiferComponent>(TEXT("Modifier Component"));
	
	
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
	int FinalDamage = Damage;
	
	// UE_LOG(LogTemp, Warning, TEXT("Damage: %d"), FinalDamage);
	
	const FString BoneName = Hit.BoneName.ToString();
	if (BoneName == FString("Bip001-Head"))
	{
		FinalDamage *= 2;

		FVector Location = GetActorLocation() +
			GetActorUpVector() * (GetCapsuleComponent()->GetScaledCapsuleHalfHeight());
		
		GetWorld()->SpawnActor<AHeadshotText>(HeadshotTextClass, Location, GetActorRotation());
		// TODO: Play distinct headshot sound
	}
	
	GiveModifier(AMoveSlowModifier::StaticClass());
	DecrementHealth(FinalDamage, Shooter);
}

// Handles healing and damage
void AEnemy::DecrementHealth(int Damage, APlayerCharacter* Shooter)
{
	if (HealthPoints - Damage <= 0)
	{
		if (!IsDead)
		{
			Shooter->IncrementGold(GoldValue);
			IsDead = true;
		}
		
		Destroy();
	}

	HealthPoints -= Damage;

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


