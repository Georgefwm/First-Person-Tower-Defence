// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"
#include "../../MyProjectProjectile.h"
#include "GameFramework/PlayerController.h"
#include "Camera/PlayerCameraManager.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"


AWeapon::AWeapon()
{
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
	
}

void AWeapon::SecondaryFire()
{
}

void AWeapon::Reload()
{
}

void AWeapon::OnEquip()
{
	this->SetActorHiddenInGame(false);
}

void AWeapon::OnUnEquip()
{
	this->SetActorHiddenInGame(true);
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