// Fill out your copyright notice in the Description page of Project Settings.


#include "PistolWeapon.h"

APistolWeapon::APistolWeapon()
{
	WeaponType = EWeaponType::Pistol;
	FireLogic = EFireLogicType::Hitscan;
	
	ClipSize = 12;
	Ammo = ClipSize;
	ReloadSpeed = 1.0;
	FireRate = 0.1;
}

void APistolWeapon::PrimaryFire()
{
	if (this->Ammo <= 0)
	{
		// TODO: play empty sound
		return;
	}
	
	Super::PrimaryFire();
	
	APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner());
	if (OwningPlayer == nullptr)
		return;

	FMinimalViewInfo CameraView;
	OwningPlayer->CalcCamera(GetWorld()->DeltaTimeSeconds, CameraView);

	FHitResult HitRes;
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);

	FVector From = CameraView.Location;
	FVector To = CameraView.Location + CameraView.Rotation.Vector() * 10000;
	
	if (GetWorld()->LineTraceSingleByChannel(HitRes, From, To, ECC_Visibility, TraceParams))
	{
		DrawDebugLine(GetWorld(), From, HitRes.ImpactPoint, FColor(255, 0, 0), false, 0.1);
		DrawDebugBox(GetWorld(), HitRes.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, false, 0.1);		
		
		AEnemy* Enemy = Cast<AEnemy>(HitRes.GetActor());
		if (Enemy)
		{
			// Tell the server about our hit result
			Enemy->HandleHit(HitRes, Damage);
		}
	}
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
