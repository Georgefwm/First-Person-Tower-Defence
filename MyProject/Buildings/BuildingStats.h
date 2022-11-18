#pragma once

#include "Engine/DataTable.h"
#include "BuildingStats.generated.h"

USTRUCT(BlueprintType)
struct FBuildingStats : public FTableRowBase
{
	
	GENERATED_BODY()
	
public:
	UPROPERTY(BlueprintReadWrite)
	FString PathToBpClass;

	UPROPERTY(BlueprintReadWrite)
	FString DisplayName;

	UPROPERTY(BlueprintReadWrite)
	FString Description;
	
	UPROPERTY(BlueprintReadWrite)
	int MaxHealthPoints;

	UPROPERTY(BlueprintReadWrite)
	float AttackRange;

	UPROPERTY(BlueprintReadWrite)
	int AttackDamage;
	
	UPROPERTY(BlueprintReadWrite)
	float AttackSpeed;

	UPROPERTY(BlueprintReadWrite)
	float BuildTime;

	UPROPERTY(BlueprintReadWrite)
	int Cost;
};
