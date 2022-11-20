// Fill out your copyright notice in the Description page of Project Settings.


#include "TowerDefenceGameState.h"
#include "Kismet/GameplayStatics.h"

ATowerDefenceGameState::ATowerDefenceGameState()
	:Super()
{
	WaveNumber = 0;
	BetweenWaveBreak = 10;
	
	MaxLives = 10;
	RemainingLives = MaxLives;

	PlayState = EPlayState::PS_Defending;
}

void ATowerDefenceGameState::BeginPlay()
{
	Super::BeginPlay();

	// Get Spawn ref
	TArray<AActor*> SpawnActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemySpawn::StaticClass(), SpawnActors);
	
	if (!SpawnActors.IsEmpty())
	{
		if (AEnemySpawn* SpawnActor = Cast<AEnemySpawn>(SpawnActors.Pop()))
			SetLevelSpawn(SpawnActor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GAMESTATE: Spawn actor not found"));
		FGenericPlatformMisc::RequestExit(false);
	}

	// Get Objective ref
	TArray<AActor*> ObjectiveActors;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(), AObjective::StaticClass(), ObjectiveActors);
	
	if (!ObjectiveActors.IsEmpty())
	{
		if (AObjective* ObjectiveActor = Cast<AObjective>(ObjectiveActors.Pop()))
			SetLevelObjective(ObjectiveActor);
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("GAMESTATE: Objective actor not found"));
		FGenericPlatformMisc::RequestExit(false);
	}

	NextPhase();
}

void ATowerDefenceGameState::UpdateGameState()
{
	if (PlayState == EPlayState::PS_Defending )
	{
		if(WaveFinished())
			NextPhase();
	}
}

void ATowerDefenceGameState::DecrementLives(int Amount)
{
	// Lose condition
	if (RemainingLives - Amount <= 0)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("You Lost :("));
		//EndGame();
	}
	
	RemainingLives -= Amount;
}

void ATowerDefenceGameState::IncrementLives(int Amount)
{
	if (RemainingLives + Amount > MaxLives)
	{
		RemainingLives = MaxLives;
	}
	
	RemainingLives += Amount;
}

float ATowerDefenceGameState::GetCountDown()
{
	if (GetWorldTimerManager().IsTimerActive(BreakTimer))
	{
		return GetWorldTimerManager().GetTimerRemaining(BreakTimer);
	}
	else
	{
		return 0;
	}
}

bool ATowerDefenceGameState::WaveFinished()
{
	if (!Spawn->WaveSpawnsRemaining())
	{
		TArray<AActor*> Actors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), AEnemy::StaticClass(), Actors);

		bool AnyAlive = false;

		
		
		for (AActor* Actor : Actors)
		{
			if(AEnemy* Enemy = StaticCast<AEnemy*>(Actor))
			{
				if (!Enemy->IsDead)
				{
					AnyAlive = true;
				}
			}
		}

		if (!AnyAlive)
			return true;
	}
	
	return false;
}

void ATowerDefenceGameState::NextPhase()
{
	if (PlayState == EPlayState::PS_Defending)
	{
		PlayState = EPlayState::PS_Planning;
		WaveNumber += 1;

		if (Spawn)
			Spawn->GenerateWave(WaveNumber);
		else
			UE_LOG(LogTemp, Error, TEXT("GAMEMODE: Spawn is null"));
		
		GetWorldTimerManager().SetTimer(BreakTimer, this, &ATowerDefenceGameState::NextPhase, BetweenWaveBreak, false);
	}
	else if (PlayState == EPlayState::PS_Planning)
	{
		PlayState = EPlayState::PS_Defending;
		
		if (Spawn)
			Spawn->StartWave();
		else
			UE_LOG(LogTemp, Error, TEXT("GAMEMODE: Spawn is null"));
	}
}