// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Buildings/Comparators/PrioritySystem.h"
#include "ClosestPriority.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API UClosestPriority : public UPrioritySystem
{
	GENERATED_BODY()

public:
	UClosestPriority();
	
	virtual AEnemy* Compare(ABuilding *Asker, TArray<AActor*> &Candidates) override;
};
