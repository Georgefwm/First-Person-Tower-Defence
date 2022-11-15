#pragma once
#include "MyProject/Enemies/Enemy.h"

#include "Modifier.generated.h"

UENUM()
enum class EModifierType : uint8
{
	MT_Buff		UMETA(DisplayName="Buff"),
	MT_DeBuff	UMETA(DisplayName="DeBuff")
};

UENUM()
enum class ECalcType : uint8
{
	CT_Tick		UMETA(DisplayName="Calculate every tick"),
	CT_Once		UMETA(DisplayName="Calculate once")
};

UCLASS()
class AModifier : public AActor
{
	GENERATED_BODY()
	
public:
	// Is the modifier good or bad for recipient
	UPROPERTY()
	EModifierType ModifierType;

	// How should the modifier be dealt with
	UPROPERTY()
	ECalcType CalculationType;

	UPROPERTY()
	AEnemy* Target;

	UPROPERTY()
	UModiferComponent* OwningComponent;

	UPROPERTY()
	float ModifierValue;

	// Can the modifier stack
	UPROPERTY()
	bool CanStack;

	// Max stacks (only applicable if CanStack == true
	UPROPERTY();
	int MaxStacks = 1;
	
	UPROPERTY()
	FTimerHandle Timer;

	// Duration of the modifiers effect
	UPROPERTY()
	float Duration;

	// Returns description of modifier
	UFUNCTION()
	virtual FString GetDescription();

	// Apply modifier to an Enemy
	UFUNCTION()
	virtual void Apply(AEnemy* Enemy, UModiferComponent* ModiferComponent);

	// Used by timer to self-remove modifier
	UFUNCTION()
	virtual void Remove();
};
