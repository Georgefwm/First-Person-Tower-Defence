// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/ModifierEffects/Modifier.h"
#include "MoveSlowModifier.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API AMoveSlowModifier : public AModifier
{
	GENERATED_BODY()

	AMoveSlowModifier();

	// Returns description of modifier
	virtual FString GetDescription() override;

	// Apply modifier to an Enemy
	virtual void Apply(AEnemy* Target, UModiferComponent* ModiferComponent) override;

	// Used by timer to self-remove modifier
	virtual void Remove() override;

	virtual void EndPlay(const EEndPlayReason::Type EndPlayReason) override;
};
