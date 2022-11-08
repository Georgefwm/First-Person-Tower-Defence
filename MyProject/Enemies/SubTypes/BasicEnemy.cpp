// Fill out your copyright notice in the Description page of Project Settings.


#include "BasicEnemy.h"


// Sets default values
ABasicEnemy::ABasicEnemy()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Create a mesh component (and position for default SKM_Manny skeleton)
	USkeletalMeshComponent* MeshComponent = GetMesh();
	MeshComponent->SetRelativeRotation(FRotator(1.9f, -19.19f, 5.2f));
	MeshComponent->SetRelativeLocation(FVector(-0.5f, -4.4f, -155.7f));
	MeshComponent->SetRelativeScale3D(FVector(0.8925, 0.8925, 0.8925));

	HealthPoints = 100;
	GoldValue = 20;
}

// Called when the game starts or when spawned
void ABasicEnemy::BeginPlay()
{
	Super::BeginPlay();
}

// Called every frame
void ABasicEnemy::Tick(float DeltaTime)
{
}

// Called to bind functionality to input
void ABasicEnemy::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	Super::SetupPlayerInputComponent(PlayerInputComponent);
}

