// Fill out your copyright notice in the Description page of Project Settings.


#include "Door.h"

#include "Components/BoxComponent.h"


// Sets default values
ADoor::ADoor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SceneRoot = CreateDefaultSubobject<USceneComponent>(TEXT("SceneRoot"));
	SetRootComponent(SceneRoot);

	Base = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("WallOpening"));
	Base->SetupAttachment(SceneRoot);
	
	DoorLeft = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorLeft"));
	DoorLeft->SetupAttachment(Base);
	
	DoorRight = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("DoorRight"));
	DoorRight->SetupAttachment(Base);
	
	OverlapComponent = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapBox"));
	OverlapComponent->SetupAttachment(Base);
	OverlapComponent->InitBoxExtent(FVector(300, 300, 300));
	OverlapComponent->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
}


void ADoor::UpdateDoorPosition(float DeltaTime)
{
	if (OpenCurve == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DOOR: Door open curve not set"));
		return;
	}

	if (DoorRight == nullptr || DoorLeft == nullptr)
	{
		UE_LOG(LogTemp, Warning, TEXT("DOOR: Doors Not set"));
		return;
	}
	
	FVector CurrentRelativeLocation = DoorRight->GetRelativeLocation();
	float NewOffset = CurrentRelativeLocation.X;

	// Evaluate boolean to get +/- (opening/closing movement)
	float const DirectionMod = (-1 + (ShouldBeOpen * 2));
	
	NewOffset = FMath::Clamp(NewOffset + (DirectionMod * (OpenSpeed * DeltaTime)), 0.0, MaxOpenOffset);

	// Evaluate curve to get nice transitions
	// BUG: Door doesnt open when curve is sufficiently complex
	NewOffset = OpenCurve->GetFloatValue(NewOffset / MaxOpenOffset) * MaxOpenOffset;

	// Set the new position of doors
	FVector NewRightLocation = CurrentRelativeLocation;
	NewRightLocation.X = NewOffset;
	
	FVector NewLeftLocation = CurrentRelativeLocation;
	NewLeftLocation.X = NewOffset * -1;
	
	DoorRight->SetRelativeLocation(NewRightLocation);
	DoorLeft->SetRelativeLocation(NewLeftLocation);
}

bool ADoor::ShouldUpdateThisTick()
{
	if (ShouldBeOpen && GetOpenPercentage() >= 1.0)
		return false;

	if (!ShouldBeOpen && GetOpenPercentage() <= 0.0)
		return false;
	
	return true;
}

float ADoor::GetOpenPercentage()
{
	return DoorRight->GetRelativeLocation().X / MaxOpenOffset;
}

void ADoor::SetLocked(bool NewLockStatus)
{
	if (NewLockStatus)
	{
		DoorLeft->SetCollisionResponseToAllChannels(ECR_Block);
		DoorRight->SetCollisionResponseToAllChannels(ECR_Block);
	}
	else
	{
		DoorLeft->SetCollisionResponseToAllChannels(ECR_Ignore);
		DoorRight->SetCollisionResponseToAllChannels(ECR_Ignore);
	}
	
	Locked = NewLockStatus;
}

// Called when the game starts or when spawned
void ADoor::BeginPlay()
{
	Super::BeginPlay();

	if (DoorRight)
		OriginalDoorOffset = 0.0;
	else
		UE_LOG(LogTemp, Warning, TEXT("DOOR: Door not found"));

	SetLocked(false);
}

// Called every frame
void ADoor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	// Check if any pawns are close to the door
	TArray<AActor*> Actors;
	OverlapComponent->GetOverlappingActors(Actors, APawn::StaticClass());
	
	if (Actors.IsEmpty())
	{
		ShouldBeOpen = false;
	}
	else
	{
		ShouldBeOpen = true;
	}
	
	// Update if we need to
	if (ShouldUpdateThisTick())
	{
		if (!Locked)
		{
			UpdateDoorPosition(DeltaTime);
		}
	}
}

