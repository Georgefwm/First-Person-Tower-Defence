// Fill out your copyright notice in the Description page of Project Settings.


#include "ModiferComponent.h"

#include "ComponentUtils.h"


void UModiferComponent::GetModifiers(TArray<AModifier*> Modifiers)
{
	
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
	FActorSpawnParameters const SpawnParameters;
	
	if (AModifier* Modifier = GetWorld()->SpawnActor<AModifier>(ModClass, SpawnParameters))
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Slowing enemy"));

		if (AEnemy* Owner = StaticCast<AEnemy*>(GetOwner()))
			Modifier->Apply(Owner, this);
	}
	
}

