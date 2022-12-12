#pragma once

#include "MyProject/Buildings/Building.h"
#include "MyProject/Enemies/Enemy.h"
#include "PrioritySystem.generated.h"

UCLASS()
class UPrioritySystem : public UObject
{
	GENERATED_BODY()
	
public:
	UPrioritySystem();

	FString Name;

	virtual AEnemy* Compare(ABuilding *Asker, TArray<AActor*> &Candidates) { return nullptr; };
};
