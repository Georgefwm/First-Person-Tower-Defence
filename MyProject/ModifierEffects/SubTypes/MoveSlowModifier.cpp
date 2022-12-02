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
	Super::Apply(Enemy, ModiferComponent);
	
	// Remove the modifier if already active on the target
	if(OwningComponent->IsModifierActive(StaticClass()))
		OwningComponent->RemoveModifierByClass(StaticClass());

	static ConstructorHelpers::FObjectFinder<UTexture2D> IconFinder(TEXT("Texture2D'/Game/Icons_Using/Snowflake/Snowflake256'"));
	Icon = IconFinder.Object;
	
	Target->SetMoveSpeed(Target->GetBaseMoveSpeed() * ModifierValue);

	GetWorldTimerManager().SetTimer(Timer, this, &AMoveSlowModifier::Remove, Duration, false);

	ModiferComponent->ActiveModifiers.Add(this);
}

void AMoveSlowModifier::Remove()
{
	Super::Remove();

	if (IsValid(Target) && IsValid(OwningComponent))
	{
		Target->SetMoveSpeed(Target->GetBaseMoveSpeed());
	}

	
}

void AMoveSlowModifier::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}


