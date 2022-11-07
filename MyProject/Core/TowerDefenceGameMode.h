﻿#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameMode.h"
#include "Objective.h"
#include "TowerDefenceGameMode.generated.h"

UCLASS(MinimalAPI)
class ATowerDefenceGameMode : public AGameMode
{
	GENERATED_BODY()

	virtual void BeginPlay() override;

public:
	ATowerDefenceGameMode();
	~ATowerDefenceGameMode() = default;

	UFUNCTION()
	void LevelComplete();
	
	UFUNCTION()
	void EndGame();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Information")
	int RemainingLives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Information")
	int MaxLives = 100;

	UFUNCTION(BlueprintCallable)
	void DecrementLives(int Amount);
	void IncrementLives(int Amount);
	
	/** Current wave (starts at zero) */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Information")
	int WaveNumber = 0;

	/** Rest time between waves length */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start params")
	int BetweenWaveBreak = 30;
	
	/** Time since game started */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Information")
	int GameTimeElapsed = 0;

	/** Time since game started */
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category = "Game Information")
	AObjective* Objective;

	const AObjective* GetLevelObjective() { return Objective; };
	void SetLevelObjective(AObjective* Obj) { Objective = Obj; };
	
};


