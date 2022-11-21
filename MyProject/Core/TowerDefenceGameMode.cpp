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
}

void ATowerDefenceGameMode::EndGame()
{
}






