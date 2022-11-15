#include "Modifier.h"

FString AModifier::GetDescription()
{
	return "";
}

void AModifier::Apply(AEnemy* Enemy, UModiferComponent* ModiferComponent)
{
	
}

void AModifier::Remove()
{
	BeginDestroy();
}
