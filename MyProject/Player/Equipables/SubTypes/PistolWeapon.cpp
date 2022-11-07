// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolWeapon.h"

APistolWeapon::APistolWeapon()
{
	WeaponType = EWeaponType::Pistol;
	FireLogic = EFireLogicType::Hitscan;

	Damage = 10;
	ClipSize = 12;
	Ammo = ClipSize;
	ReloadSpeed = 1.0;
	FireRate = 0.1;
}

void APistolWeapon::PrimaryFirePressed()
{
	Super::PrimaryFirePressed();

	if (GetWorld()->GetTimeSeconds() - LastFireTime < FireRate)
		return;
	
	AWeapon::FireHitScan();
}

void APistolWeapon::PrimaryFireReleased()
{
}

void APistolWeapon::SecondaryFire()
{
	Super::SecondaryFire();
}

void APistolWeapon::Reload()
{
	Super::Reload();
}

void APistolWeapon::OnEquip()
{
	Super::OnEquip();
}

void APistolWeapon::OnUnEquip()
{
	Super::OnUnEquip();
}

void APistolWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void APistolWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}
