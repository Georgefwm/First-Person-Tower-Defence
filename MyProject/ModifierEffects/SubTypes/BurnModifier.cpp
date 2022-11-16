// Fill out your copyright notice in the Description page of Project Settings.


#include "BurnModifier.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "MyProject/ModifierEffects/ModiferComponent.h"

ABurnModifier::ABurnModifier()
{
	ModifierValue = 0.1;
	Duration = 5.0;
}

FString ABurnModifier::GetDescription()
{
	return Super::GetDescription();

}

void ABurnModifier::Apply(AEnemy* Enemy, UModiferComponent* ModiferComponent)
{
	if (!ModiferComponent->ActiveModifiers.IsEmpty())
	{
		for (int Index = 0; Index < ModiferComponent->ActiveModifiers.Num(); Index++)
		{
			if (!ModiferComponent->ActiveModifiers.IsValidIndex(Index))
				continue;
			
			AModifier* Mod = ModiferComponent->ActiveModifiers[Index];
			
			if (Mod->IsA(GetClass()))
			{
				GetWorldTimerManager().ClearTimer(Mod->Timer);
				ModiferComponent->ActiveModifiers.Remove(Mod);
				Mod->Destroy();
			}
		}
	}
	
	Target = Enemy;
	OwningComponent = ModiferComponent;

	GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, Target->GetActorNameOrLabel());

	Target->SetMoveSpeed(Target->GetBaseMoveSpeed() * ModifierValue);

	GetWorldTimerManager().SetTimer(Timer, this, &ABurnModifier::Remove, Duration, true);

	ModiferComponent->ActiveModifiers.Add(this);
}

void ABurnModifier::Remove()
{
	if (Target && OwningComponent)
	{
		OwningComponent->ActiveModifiers.Remove(this);
	}
	Destroy();
}
