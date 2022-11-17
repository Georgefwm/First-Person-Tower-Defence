// Fill out your copyright notice in the Description page of Project Settings.


#include "ModiferComponent.h"
#include "Engine/World.h"


void UModiferComponent::GetModifiers(TArray<AModifier*> Modifiers)
{
	for (AModifier* Mod : ActiveModifiers)
	{
		Modifiers.Add(Mod);
	}
}

void UModiferComponent::RemoveModifier(AModifier* Mod)
{
	if (Mod == nullptr)
		return;
	
	// Clear timer if it is running
	Mod->GetWorldTimerManager().ClearTimer(Mod->Timer);
				
	ActiveModifiers.Remove(Mod);
	Mod->Destroy();
}

void UModiferComponent::RemoveModifierByClass(UClass* ModClass)
{
	if (ActiveModifiers.IsEmpty())
		return;
	
	for (int Index = 0; Index < ActiveModifiers.Num(); Index++)
	{
		AModifier* Mod = ActiveModifiers[Index];

		if (Mod == nullptr)
			return;
		
		if (Mod->IsA(ModClass))
		{			
			ActiveModifiers.Remove(Mod);
			Mod->Remove();
		}
	}
}

void UModiferComponent::RemoveAllModifiers()
{
	// Reverse order coz faster?
	for (int Index = ActiveModifiers.Num(); Index > 0; Index--)
	{
		AModifier* Mod = ActiveModifiers[Index];
		
		if (Mod == nullptr)
			return;
		
		Mod->Remove();
	}
}

bool UModiferComponent::IsModifierActive(UClass* ModClass)
{
	return ContainsObjectOfClass(ActiveModifiers, ModClass, true);
}

// Called when the game starts
void UModiferComponent::BeginPlay()
{
	Super::BeginPlay();
}


// Called every frame
void UModiferComponent::TickComponent(float DeltaTime, ELevelTick TickType,
                                      FActorComponentTickFunction* ThisTickFunction)
{
	Super::TickComponent(DeltaTime, TickType, ThisTickFunction);
}

void UModiferComponent::ApplyModifier(UClass* ModClass)
{
	if (!AcceptingModifiers)
		return;
	
	FActorSpawnParameters const SpawnParameters;
	
	if (AModifier* Modifier = GetWorld()->SpawnActor<AModifier>(ModClass, SpawnParameters))
	{
		if (AEnemy* Owner = StaticCast<AEnemy*>(GetOwner()))
			Modifier->Apply(Owner, this);
	}
	
}

