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

	bReplicates = true;

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

bool AWeapon::Server_HitscanApplyDamage_Validate(FHitResult Hit, double HitDamage)
{
	return true;
}

void AWeapon::Server_HitscanApplyDamage_Implementation(FHitResult Hit, double HitDamage)
{
	AEnemy* Enemy = Cast<AEnemy>(Hit.GetActor());
	if (Enemy)
	{
		Enemy->ApplyDamage(HitDamage);
	}
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

}

void AWeapon::OnUnEquip()
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