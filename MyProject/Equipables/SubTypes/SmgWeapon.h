﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../Weapon.h"
#include "SmgWeapon.generated.h"

/**
 * 
 */
UCLASS()
class MYPROJECT_API ASmgWeapon : public AWeapon
{

	GENERATED_BODY()
	
public:
	// Sets default values for this actor's properties
	ASmgWeapon();

	/** Make the weapon Fire a Projectile */
	virtual void PrimaryFirePressed() override;

	/** Stop firing */
	virtual void PrimaryFireReleased() override;
	
	/** Make the weapon Fire a Projectile */
	virtual void SecondaryFirePressed() override;

	/** Reload Weapon */
	virtual void Reload() override;

	virtual void OnEquip() override;
	
	virtual void OnUnEquip() override;

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
