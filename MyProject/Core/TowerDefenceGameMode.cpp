#include "TowerDefenceGameMode.h"

#include "TowerDefenceGameState.h"
#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/PlayerCharacter.h"

void ATowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();
}

ATowerDefenceGameMode::ATowerDefenceGameMode()
	: Super()
{
	GameStateClass = ATowerDefenceGameState::StaticClass();
	
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/PlayerCharacterBP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	

	// may be jank
	//TArray<AActor*> FoundActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), Objective->GetClass(), FoundActors);
	//this->SetLevelObjective(Cast<AObjective>(FoundActors[0]));
}

void ATowerDefenceGameMode::LevelComplete()
{
}

void ATowerDefenceGameMode::EndGame()
{
}

void ATowerDefenceGameMode::DecrementLives(int Amount)
{
	// Lose condition
	if (RemainingLives - Amount <= 0)
	{
		EndGame();
	}
	
	RemainingLives -= Amount;
}

void ATowerDefenceGameMode::IncrementLives(int Amount)
{
	if (RemainingLives + Amount > MaxLives)
	{
		RemainingLives = MaxLives;
	}
	
	RemainingLives += Amount;
}

