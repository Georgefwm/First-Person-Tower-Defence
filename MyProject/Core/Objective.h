// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Objective.generated.h"

UCLASS()
class MYPROJECT_API AObjective : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AObjective();
	
	/** HP at game start */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start params")
	int StartingHealthPoints = 100;

	/** HP during game */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Start params")
	int CurrentHealthPoints;


protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintCallable, BlueprintGetter)
	inline int GetHP() { return CurrentHealthPoints; };

	UFUNCTION(BlueprintCallable, BlueprintSetter)
	inline void SetHP(int Hp) { CurrentHealthPoints = Hp; };

};
