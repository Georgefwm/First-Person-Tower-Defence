// Fill out your copyright notice in the Description page of Project Settings.


#include "Weapon.h"

#include "WeaponStats.h"
#include "Kismet/GameplayStatics.h"
#include "Components/SkeletalMeshComponent.h"
#include "MyProject/ModifierEffects/SubTypes/MoveSlowModifier.h"


AWeapon::AWeapon()
{
	WeaponMesh = CreateDefaultSubobject<USkeletalMeshComponent>("SkeletalMeshComponent");
	RootComponent = WeaponMesh;

	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	SetActorHiddenInGame(false);
}

void AWeapon::SetupStats(FName RowName)
{
	if (WeaponDataTable == nullptr)
	{
		//UE_LOG(LogTemp, Error, TEXT("Data table not set for weapon"));	
		return;
	}	
	
	if (FWeaponStats* OutRow = WeaponDataTable->FindRow<FWeaponStats>(RowName, "WeaponStats", true))
	{
		Damage = OutRow->Damage;
		ClipSize = OutRow->ClipSize;
		ReloadSpeed = OutRow->ReloadSpeed;
		FireRate = OutRow->FireRate;
	}
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to read weapon data table"));
		return;
	}
	Ammo = ClipSize;
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
		if (EmptySound)
			UGameplayStatics::PlaySound2D(GetWorld(), EmptySound, VolumeModifier, 1, 0);
		
		return;
	}

	GetWorldTimerManager().ClearTimer(ReloadTimer);

	if (WeaponOwner == nullptr)
		return;

	if (FireSound)
		UGameplayStatics::PlaySound2D(GetWorld(), FireSound, VolumeModifier, 1, 0);
	
	FMinimalViewInfo CameraView;
	WeaponOwner->CalcCamera(GetWorld()->DeltaTimeSeconds, CameraView);

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
			Enemy->HandleHit(HitRes, Damage, WeaponOwner);
		}
	}
	
	this->Ammo -= 1;
	LastFireTime = GetWorld()->GetTimeSeconds();
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

	// if (ReloadAnimation)
	// 	WeaponMesh->PlayAnimation(ReloadAnimation, false);

	if (EmptySound)
		UGameplayStatics::PlaySound2D(GetWorld(), ReloadSound, VolumeModifier, 1, 0);
	
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