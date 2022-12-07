// Fill out your copyright notice in the Description page of Project Settings.


#include "BurnModifier.h"

#include "GameFramework/CharacterMovementComponent.h"
#include "MyProject/ModifierEffects/ModiferComponent.h"

ABurnModifier::ABurnModifier()
{
	ModifierValue = 5;  // Burn damage
	TickRate = 0.3;
	Duration = 5.0;
}

FString ABurnModifier::GetDescription()
{
	return Super::GetDescription();

}

void ABurnModifier::Apply(AEnemy* Enemy, UModiferComponent* ModiferComponent)
{
	Super::Apply(Enemy, ModiferComponent);

	// Remove the modifier if already active on the target
	if(OwningComponent->IsModifierActive(StaticClass()))
		OwningComponent->RemoveModifierByClass(StaticClass());
	
	// Set burn repeating timer
	GetWorldTimerManager().SetTimer(Timer, this, &ABurnModifier::BurnTarget, TickRate,
		true,TickRate);
	
	ModiferComponent->ActiveModifiers.Add(this);
}

void ABurnModifier::BurnTarget()
{
	Target->DecrementHealth(ModifierValue);
}

void ABurnModifier::BeginPlay()
{
	Super::BeginPlay();

	SetLifeSpan(Duration);
}

void ABurnModifier::Remove()
{
	Super::Remove();
}

void ABurnModifier::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
