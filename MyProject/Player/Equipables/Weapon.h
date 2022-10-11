﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum EWeaponType
{
	Shotgun UMETA(Displayname, "Shotgun"),
	Rifle UMETA(Displayname, "Rifle"),
	Pistol UMETA(Displayname, "Pistol"),
	BuildTool UMETA(Displayname, "Build Tool")
};

UCLASS()
class MYPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties */
	AWeapon();

	void BeginPlay() override;

	UPROPERTY(EditDefaultsOnly, Category="Asthetic")
	TEnumAsByte<EWeaponType> WeaponType;

	/** Weapon skeletal mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asthetic")
	USkeletalMeshComponent* WeaponMesh;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asthetic")
	USoundBase* FireSound;
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asthetic")
	UAnimationAsset* FireAnimation;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asthetic")
	UAnimationAsset* ReloadAnimation;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asthetic")
	FVector MuzzleOffset;
	
	/** Attaches the actor to a mesh component */
	// UFUNCTION(BlueprintCallable, Category="Backend")
	// void AttachToComponent(USkeletalMeshComponent* TargetMesh);

	UPROPERTY(VisibleAnywhere)
	bool IsActiveWeapon;
	
	/** Current Ammo */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int Ammo;

	/** Clip Size */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int ClipSize;

	/** Reload Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	double ReloadSpeed;

	/** Max Fire Rate */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float FireRate;

	/** Reload Speed */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float FireCooldown;

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EWeaponType> GetWeaponType() { return WeaponType; }

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void PrimaryFire();
	
	UFUNCTION(Server, Reliable, WithValidation)
	virtual void Server_PrimaryFire();
	virtual bool Server_PrimaryFire_Validate();
	virtual void Server_PrimaryFire_Implementation();

	UFUNCTION(NetMulticast, Reliable, WithValidation)
	virtual void Multi_PrimaryFire();
	virtual bool Multi_PrimaryFire_Validate();
	virtual void Multi_PrimaryFire_Implementation();
	
	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void SecondaryFire();

	/** Reload Weapon */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void Reload();

	UFUNCTION()
	virtual void OnEquip();
	
	UFUNCTION()
	virtual void OnUnEquip();
	
};