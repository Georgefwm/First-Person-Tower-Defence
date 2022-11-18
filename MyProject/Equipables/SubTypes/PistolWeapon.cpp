// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolWeapon.h"

#include "MyProject/Equipables/WeaponStats.h"

APistolWeapon::APistolWeapon()
{
	WeaponType = EWeaponType::Pistol;
	FireLogic = EFireLogicType::Hitscan;
	WeaponName = FName(TEXT("pistol"));

	// Pull weapon stats from data table
	SetupStats(WeaponName);
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

void APistolWeapon::SecondaryFirePressed()
{
	Super::SecondaryFirePressed();
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
