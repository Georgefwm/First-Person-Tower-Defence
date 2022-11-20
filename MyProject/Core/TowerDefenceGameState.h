// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "EnemySpawn.h"
#include "Objective.h"
#include "GameFramework/GameState.h"
#include "TowerDefenceGameState.generated.h"

UENUM()
enum class EPlayState : uint8
{
	PS_Planning		UMETA(DisplayName="Planning"),
	PS_Defending	UMETA(DisplayName="Defending")
};

/**
 * 
 */
UCLASS()
class MYPROJECT_API ATowerDefenceGameState : public AGameStateBase
{
	GENERATED_BODY()

public:
	ATowerDefenceGameState();

	virtual void BeginPlay() override;
	
	UPROPERTY()
	EPlayState PlayState;

	UFUNCTION(BlueprintCallable)
	EPlayState GetPlayState() const { return PlayState; }

	UFUNCTION(BlueprintCallable)
	void UpdateGameState();

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Information")
	int RemainingLives;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Game Information")
	int MaxLives;

	UFUNCTION(BlueprintCallable)
	void DecrementLives(int Amount);

	UFUNCTION(BlueprintCallable)
	void IncrementLives(int Amount);

	UFUNCTION(BlueprintCallable)
	int GetRemainingLives() const { return RemainingLives; }

	/** Current wave (starts at zero) */
	UPROPERTY(VisibleAnywhere, Category = "Game Information")
	int WaveNumber;

	UFUNCTION(BlueprintCallable)
	int GetWaveNumber() const { return WaveNumber; }

	/** Rest time between waves length */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start params")
	int BetweenWaveBreak;

	UPROPERTY()
	FTimerHandle BreakTimer;

	UFUNCTION(BlueprintCallable)
	float GetCountDown();

	UFUNCTION()
	bool WaveFinished();

	UFUNCTION()
	void NextPhase();
	
	UPROPERTY()
	AObjective* Objective;

	UFUNCTION()
	const AObjective* GetLevelObjective() const { return Objective; };

	UFUNCTION()
	void SetLevelObjective(AObjective* Obj) { Objective = Obj; };

	UPROPERTY()
	AEnemySpawn* Spawn;

	UFUNCTION()
	AEnemySpawn* GetLevelSpawn() const { return Spawn; };

	UFUNCTION()
	void SetLevelSpawn(AEnemySpawn* Obj) { Spawn = Obj; };
};
