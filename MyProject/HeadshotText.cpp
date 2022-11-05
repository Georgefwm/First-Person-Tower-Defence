// Fill out your copyright notice in the Description page of Project Settings.


#include "HeadshotText.h"

AHeadshotText::AHeadshotText()
{
}

void AHeadshotText::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}

const FVector& AHeadshotText::GetAnchorLocation()
{
	return AnchorLocation;
}

void AHeadshotText::BeginPlay()
{
	Super::BeginPlay();

	AnchorLocation = GetActorLocation();
}
