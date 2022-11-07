// Fill out your copyright notice in the Description page of Project Settings.


#include "SmgWeapon.h"

ASmgWeapon::ASmgWeapon()
{
	WeaponType = EWeaponType::Smg;
	FireLogic = EFireLogicType::Hitscan;

	Damage = 8;
	ClipSize = 24;
	Ammo = ClipSize;
	ReloadSpeed = 1.0;
	FireRate = 0.1;
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

void ASmgWeapon::SecondaryFire()
{
	Super::SecondaryFire();
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
