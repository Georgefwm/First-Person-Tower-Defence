// Fill out your copyright notice in the Description page of Project Settings.


#include "MoveSlowModifier.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "MyProject/ModifierEffects/ModiferComponent.h"

AMoveSlowModifier::AMoveSlowModifier()
{
	ModifierValue = 0.1;
	Duration = 5.0;
}

FString AMoveSlowModifier::GetDescription()
{
	return Super::GetDescription();

}

void AMoveSlowModifier::Apply(AEnemy* Enemy, UModiferComponent* ModiferComponent)
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

	GetWorldTimerManager().SetTimer(Timer, this, &AMoveSlowModifier::Remove, Duration, false);

	ModiferComponent->ActiveModifiers.Add(this);
}

void AMoveSlowModifier::Remove()
{
	if (Target && OwningComponent)
	{
		Target->SetMoveSpeed(Target->GetBaseMoveSpeed());
		OwningComponent->ActiveModifiers.Remove(this);
	}
	Destroy();
}
