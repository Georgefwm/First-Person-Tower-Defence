// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Modifier.h"
#include "Components/ActorComponent.h"
#include "ModiferComponent.generated.h"

/**
 * Manages modifiers of the attached actor
 */
UCLASS(ClassGroup=(Custom), meta=(BlueprintSpawnableComponent))
class MYPROJECT_API UModiferComponent : public UActorComponent
{
	GENERATED_BODY()

public:
	// Sets default values for this component's properties

	UPROPERTY(VisibleAnywhere)
	TArray<AModifier*> ActiveModifiers;

	UFUNCTION()
	void GetModifiers(TArray<AModifier*> Modifiers);

	UPROPERTY(VisibleAnywhere)
	AEnemy* OwningEnemy;


protected:
	// Called when the game starts
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void TickComponent(float DeltaTime, ELevelTick TickType,
	                           FActorComponentTickFunction* ThisTickFunction) override;
	

	// Takes the class of the modifier to be created
	UFUNCTION()
	void ApplyModifier(UClass* ModClass);
	

	
};
