// Fill out your copyright notice in the Description page of Project Settings.


#include "SmgWeapon.h"

#include "MyProject/Equipables/WeaponStats.h"

ASmgWeapon::ASmgWeapon()
{
	WeaponType = EWeaponType::Smg;
	FireLogic = EFireLogicType::Hitscan;
	WeaponName = FName(TEXT("smg"));

	// Pull weapon stats from data table
	SetupStats(WeaponName);
}

void ASmgWeapon::PrimaryFirePressed()
{
	Super::PrimaryFirePressed();

	if (GetWorldTimerManager().IsTimerActive(ReloadTimer))
	{
		return;
	}
	
	// For instant first shot
	AWeapon::FireHitScan();
	
	GetWorldTimerManager().SetTimer(FireTimer, this, &AWeapon::FireHitScan, this->FireRate, true);
}

void ASmgWeapon::PrimaryFireReleased()
{
	Super::PrimaryFireReleased();

	GetWorldTimerManager().ClearTimer(FireTimer);
}

void ASmgWeapon::SecondaryFirePressed()
{
	Super::SecondaryFirePressed();
}

void ASmgWeapon::Reload()
{
	Super::Reload();
}

void ASmgWeapon::OnEquip()
{
	Super::OnEquip();
}

void ASmgWeapon::OnUnEquip()
{
	Super::OnUnEquip();
}

void ASmgWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void ASmgWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
