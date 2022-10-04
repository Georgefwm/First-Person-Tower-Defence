// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <GameFramework/Character.h>

#include "CoreMinimal.h"
#include "../Core/TowerDefenceGameMode.h"
#include "GameFramework/Actor.h"
#include "Enemy.generated.h"


UCLASS()
class MYPROJECT_API AEnemy : public ACharacter
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AEnemy();

protected:
	
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int MaxHealthPoints = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int HealthPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int Speed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int AttackDamage;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	AActor* Target;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHPUpdate();

	UFUNCTION()
	void ApplyDamage(int Damage);

	UFUNCTION(BlueprintCallable)
	int GetHP() { return this->HealthPoints; }

	UFUNCTION(BlueprintCallable)
	int GetMaxHP() { return this->MaxHealthPoints; }
};
