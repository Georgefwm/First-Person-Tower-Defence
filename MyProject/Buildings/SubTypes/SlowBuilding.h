// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Building.h"
#include "NiagaraComponent.h"
#include "SlowBuilding.generated.h"

/**
 * Doesnt do damage, but slows enemies movement
 */
UCLASS()
class MYPROJECT_API ASlowBuilding : public ABuilding
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ASlowBuilding();

	UPROPERTY(EditDefaultsOnly, Category = "Animation - Slow Building")
	UNiagaraSystem* IceEffectNiagaraSystem;

	UPROPERTY()
	UBoxComponent* HitBox;
	
	virtual FVector GetSearchPosition() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
	
	virtual void Build(float DeltaTime) override;

	virtual void Attack(float DeltaTime) override;

	virtual void CheckForNewTarget() override;
};
