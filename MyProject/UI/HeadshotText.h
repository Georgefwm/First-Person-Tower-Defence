// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "HeadshotText.generated.h"

/**
 * 
 */
UCLASS(Abstract)
class MYPROJECT_API AHeadshotText : public AActor
{
	GENERATED_BODY()

public:
	AHeadshotText();

	UFUNCTION(BlueprintImplementableEvent, Category="HeadshotText")
	void Initialise();

	virtual void Tick(float DeltaSeconds) override;

	const FVector& GetAnchorLocation();

protected:
	virtual void BeginPlay() override;

private:
	FVector AnchorLocation;

	
};
