#pragma once

#include "CoreMinimal.h"
#include "EnemySpawn.h"
#include "GameFramework/GameMode.h"
#include "Objective.h"
#include "TowerDefenceGameMode.generated.h"


UCLASS(MinimalAPI)
class ATowerDefenceGameMode : public AGameModeBase
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	ATowerDefenceGameMode();
	
	UFUNCTION()
	void EndGame();
};


