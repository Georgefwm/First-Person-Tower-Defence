#include "TowerDefenceGameMode.h"

#include "Blueprint/UserWidget.h"
#include "Kismet/GameplayStatics.h"
#include "../Player/PlayerCharacter.h"

void ATowerDefenceGameMode::BeginPlay()
{
	Super::BeginPlay();

	APlayerCharacter* Player = Cast<APlayerCharacter>(UGameplayStatics::GetPlayerPawn(this, 0));
}

ATowerDefenceGameMode::ATowerDefenceGameMode()
	: Super()
{

	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Player/PlayerCharacterBP"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;
	

	// may be jank
	//TArray<AActor*> FoundActors;
	//UGameplayStatics::GetAllActorsOfClass(GetWorld(), Objective->GetClass(), FoundActors);
	//this->SetLevelObjective(Cast<AObjective>(FoundActors[0]));
}

