// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include <stdbool.h>
#include <GameFramework/Character.h>

#include "CoreMinimal.h"
#include "MyProject/HeadshotText.h"
#include "MyProject/Player/PlayerCharacter.h"
#include "Enemy.generated.h"

class UModiferComponent;
class AModifier;

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

	UPROPERTY()
	UModiferComponent* ModifierComponent;

	

	// Health cannot go above this (yet)
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int MaxHealthPoints = 100;
	
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int HealthPoints;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int MoveSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int AttackSpeed;

	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int AttackDamage;

	// Current target, objective or player
	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	AActor* Target;
	
	// Gold earned by defeating the enemy
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int GoldValue;
	
	UPROPERTY(BlueprintReadOnly, Category="Combat text")
	TSubclassOf<AHeadshotText> HeadshotTextClass;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat text")
	// TSubclassOf<AHeadshotText> DamageNumberTextClass;

public:
	UPROPERTY()
	APlayerCharacter* LastDamageDealer;

	// Used to stop buildings interacting with this Enemy during UEs cleanup process
	UPROPERTY()
	bool IsDead = false;

	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// Used by hp widget (may be redundant)
	UFUNCTION(BlueprintImplementableEvent)
	void OnHPUpdate();

	// Executes behaviour related to hit (damage multipliers, logging, etc)
	UFUNCTION()
	void HandleHit(FHitResult Hit, int Damage, APlayerCharacter* Shooter);

	UFUNCTION(BlueprintImplementableEvent)
	void OnDieBp();

	// Sets Max health and refills it
	UFUNCTION()
	void ModifyMaxHealth(int NewHealth);

	// Increase health (heal)
	UFUNCTION()
	void IncrementHealth(int Healing);

	// Decrease health (damage)
	UFUNCTION()
	void DecrementHealth(int Damage);

	// Get current health, used by HUD widgets
	UFUNCTION(BlueprintCallable)
	int GetHP() { return this->HealthPoints; }

	// Get max health, used by HUD widgets
	UFUNCTION(BlueprintCallable)
	int GetMaxHP() { return this->MaxHealthPoints; }

	// Used to keep record of base move speed, used by movement effecting modifiers
	UFUNCTION()
	int GetBaseMoveSpeed() const { return MoveSpeed; };

	// Change real move speed
	void SetMoveSpeed(float Speed);

	// Returns a list of modifiers currently effecting this Enemy
	UFUNCTION(BlueprintCallable)
	TArray<AModifier*> GetModifiers();

	// Gives this enemy a modifier
	UFUNCTION()
	virtual void GiveModifier(UClass* ModClass);

	UFUNCTION(BlueprintCallable)
	void SetDead() { IsDead = true; };
	
	UFUNCTION()
	void Die();
};



