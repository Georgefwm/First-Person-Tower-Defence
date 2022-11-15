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

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	AActor* Target;

	UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Enemy base Properties")
	int GoldValue;

	UPROPERTY()
	bool IsDead = false;

	UPROPERTY(BlueprintReadOnly, Category="Combat text")
	TSubclassOf<AHeadshotText> HeadshotTextClass;

	// UPROPERTY(VisibleAnywhere, BlueprintReadWrite, Category="Combat text")
	// TSubclassOf<AHeadshotText> DamageNumberTextClass;

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	UFUNCTION(BlueprintImplementableEvent)
	void OnHPUpdate();

	UFUNCTION()
	void HandleHit(FHitResult Hit, int Damage, APlayerCharacter* Shooter);

	UFUNCTION()
	void IncrementHealth(int Healing);

	UFUNCTION()
	void DecrementHealth(int Damage, APlayerCharacter* Shooter);

	UFUNCTION(BlueprintCallable)
	int GetHP() { return this->HealthPoints; }

	UFUNCTION(BlueprintCallable)
	int GetMaxHP() { return this->MaxHealthPoints; }

	UFUNCTION()
	int GetBaseMoveSpeed() const { return MoveSpeed; };

	void SetMoveSpeed(float Speed);

	UFUNCTION()
	void GetModifiers(TArray<AModifier*> Mods);

	UFUNCTION()
	virtual void GiveModifier(UClass* ModClass);
};
