// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../../MyProjectProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"


AWeapon::AWeapon()
{
	// Server
	bReplicates = true;
	
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");

	ClipSize = 10;
	Ammo = ClipSize;
	ReloadSpeed = 1.0;
	FireRate = 1.0;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::PrimaryFire()
{
	Server_PrimaryFire();
	
}

bool AWeapon::Server_PrimaryFire_Validate()
{
	return true;
}

void AWeapon::Server_PrimaryFire_Implementation()
{
	Multi_PrimaryFire();
}

bool AWeapon::Multi_PrimaryFire_Validate()
{
	return true;
}

void AWeapon::Multi_PrimaryFire_Implementation()
{
	WeaponMesh->PlayAnimation(FireAnimation, false);
}

void AWeapon::SecondaryFire()
{
}

void AWeapon::Reload()
{
	WeaponMesh->PlayAnimation(ReloadAnimation, false);
}

void AWeapon::OnEquip()
{
	SetActorHiddenInGame(false);
	IsActiveWeapon = true;

	Server_OnEquip();
}

bool AWeapon::Server_OnEquip_Validate()
{
	return true;
}

void AWeapon::Server_OnEquip_Implementation()
{
	Multi_OnEquip();
}

bool AWeapon::Multi_OnEquip_Validate()
{
	return true;
}

void AWeapon::Multi_OnEquip_Implementation()
{
	SetActorHiddenInGame(false);
	IsActiveWeapon = true;
}

void AWeapon::OnUnEquip()
{
	SetActorHiddenInGame(true);
	IsActiveWeapon = false;

	Server_OnUnEquip();
}

bool AWeapon::Server_OnUnEquip_Validate()
{
	return true;
}

void AWeapon::Server_OnUnEquip_Implementation()
{
	Multi_OnUnEquip();
}

bool AWeapon::Multi_OnUnEquip_Validate()
{
	return true;
}

void AWeapon::Multi_OnUnEquip_Implementation()
{
	SetActorHiddenInGame(true);
	IsActiveWeapon = false;
}



// FHitResult AWeapon::DoLineTrace()
// {
// 	
// }

// void AWeapon::AttachToComponent(USkeletalMeshComponent* TargetMesh)
// {
// 	
// 	if(TargetMesh != nullptr)
// 	{
// 		// Attach the weapon to the First Person Character
// 		const FAttachmentTransformRules AttachmentTransform(EAttachmentRule::SnapToTarget, true);
// 		GetOwner()->AttachToComponent(TargetMesh, AttachmentTransform, FName(TEXT("GripPoint")));
// 	}
// }