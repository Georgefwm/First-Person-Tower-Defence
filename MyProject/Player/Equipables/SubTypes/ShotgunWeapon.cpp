// Fill out your copyright notice in the Description page of Project Settings.


#include "ShotgunWeapon.h"


// Sets default values
AShotgunWeapon::AShotgunWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	//LastFireTime = GetWorld()->TimeSeconds;
	
	ClipSize = 8;
	Ammo = ClipSize;
	ReloadSpeed = 2.0;
	FireRate = 0.8f;
}

void AShotgunWeapon::PrimaryFire()
{
	if (this->FireCooldown <= 0)
	{
		WeaponMesh->PlayAnimation(FireAnimation, false);
	}
	else
	{
		// stop bes things happening if tower doesnt attack for a looooong time
		if (this->FireCooldown <= 0) this->FireCooldown = -1;
		else this->FireCooldown -= GetWorld()->DeltaTimeSeconds;;
	}
	
	
}

void AShotgunWeapon::SecondaryFire()
{
	Super::SecondaryFire();
}

void AShotgunWeapon::Reload()
{
	WeaponMesh->PlayAnimation(ReloadAnimation, false);
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

