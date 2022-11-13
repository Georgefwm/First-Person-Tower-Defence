#pragma once

#include "Engine/DataTable.h"
#include "WeaponStats.generated.h"

USTRUCT(BlueprintType)
struct FWeaponStats : public FTableRowBase
{
	
	GENERATED_BODY()
	
public:	
	UPROPERTY(BlueprintReadWrite)
	int Damage;

	UPROPERTY(BlueprintReadWrite)
	int ClipSize;

	UPROPERTY(BlueprintReadWrite)
	float ReloadSpeed;
	
	UPROPERTY(BlueprintReadWrite)
	float FireRate;
};
