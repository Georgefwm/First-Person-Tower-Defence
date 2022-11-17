#include "Modifier.h"

#include "ModiferComponent.h"

FString AModifier::GetDescription()
{
	return "";
}

void AModifier::Apply(AEnemy* Enemy, UModiferComponent* ModiferComponent)
{
	Target = Enemy;
	OwningComponent = ModiferComponent;
}

void AModifier::Remove()
{
	if (GetWorldTimerManager().IsTimerActive(Timer))
	{
		GetWorldTimerManager().ClearTimer(Timer);
	}
}

void AModifier::EndPlay(const EEndPlayReason::Type EndPlayReason)
{
	Super::EndPlay(EndPlayReason);
}
