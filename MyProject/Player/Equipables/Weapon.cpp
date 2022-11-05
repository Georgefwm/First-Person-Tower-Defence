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

void AWeapon::PrimaryFirePressed()
{
	
}

void AWeapon::PrimaryFireReleased()
{
	
}

void AWeapon::FireHitScan()
{
	if (this->Ammo <= 0)
	{
		// TODO: play empty sound
		if (EmptySound)
			UGameplayStatics::PlaySound2D(GetWorld(), EmptySound, 1, 1, 0);
		return;
	}
	
	APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner());
	if (OwningPlayer == nullptr)
		return;

	if (EmptySound)
		UGameplayStatics::PlaySound2D(GetWorld(), FireSound, 1, 1, 0);
	
	this->Ammo -= 1;

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
	
		if (AEnemy* Enemy = Cast<AEnemy>(HitRes.GetActor()))
		{
			Enemy->HandleHit(HitRes, this->Damage);
		}
	}
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

	if (ReloadAnimation)
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
	GetWorldTimerManager().ClearTimer(FireTimer);
	
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