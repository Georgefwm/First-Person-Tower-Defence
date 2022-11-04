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
	RootComponent = WeaponMesh;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;

	Damage = 10;
	ClipSize = 10;
	Ammo = ClipSize;
	ReloadSpeed = 1.0;
	FireRate = 1.0;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(false);
}

void AWeapon::PrimaryFire()
{
	WeaponMesh->PlayAnimation(FireAnimation, false);
	this->Ammo -= 1;
}



void AWeapon::SecondaryFire()
{
}

void AWeapon::Reload()
{
	if (GetWorldTimerManager().IsTimerActive(ReloadTimer) || this->Ammo == this->ClipSize)
	{
		return;
	}
	
	WeaponMesh->PlayAnimation(ReloadAnimation, false);
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeapon::ApplyReload, this->ReloadSpeed, false);
}

void AWeapon::ApplyReload()
{
	this->Ammo = this->ClipSize;
}

void AWeapon::OnEquip()
{
	SetActorHiddenInGame(false);
	IsActiveWeapon = true;
}

void AWeapon::OnUnEquip()
{
	GetWorldTimerManager().ClearTimer(ReloadTimer);
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