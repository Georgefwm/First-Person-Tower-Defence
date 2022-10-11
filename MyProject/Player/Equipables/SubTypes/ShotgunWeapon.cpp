// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"


// Sets default values
AShotgunWeapon::AShotgunWeapon()
{
	WeaponType = EWeaponType::Shotgun;
	//LastFireTime = GetWorld()->TimeSeconds;
	
	ClipSize = 8;
	Ammo = ClipSize;
	ReloadSpeed = 2.0;
	FireRate = 0.8f;
}

void AShotgunWeapon::PrimaryFire()
{
	Super::PrimaryFire();
}

void AShotgunWeapon::SecondaryFire()
{
	Super::SecondaryFire();
}

void AShotgunWeapon::Reload()
{
	Super::Reload();
}

void AShotgunWeapon::OnEquip()
{
	Super::OnEquip();
}

void AShotgunWeapon::OnUnEquip()
{
	Super::OnUnEquip();
}

// Called when the game starts or when spawned
void AShotgunWeapon::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void AShotgunWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

