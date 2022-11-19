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
}

void ATowerDefenceGameMode::EndGame()
{
}






