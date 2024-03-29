﻿// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Player/PlayerCharacter.h"
#include "Engine/DataTable.h"
#include "GameFramework/Actor.h"
#include "MyProject/Enemies/Enemy.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum EWeaponType
{
	Shotgun		UMETA(Displayname, "Shotgun"),
	Rifle		UMETA(Displayname, "Rifle"),
	Pistol		UMETA(Displayname, "Pistol"),
	Smg			UMETA(Displayname, "SMG"),
	BuildTool	UMETA(Displayname, "Build Tool")
};

UENUM(BlueprintType)
enum EFireLogicType
{
	Hitscan		UMETA(Displayname, "Hitscan"),
	Projectile	UMETA(Displayname, "Projectile")
};

UCLASS()
class MYPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties */
	AWeapon();

	void BeginPlay() override;

	UPROPERTY(VisibleAnywhere, Category="Asthetic")
	TEnumAsByte<EWeaponType> WeaponType;

	UPROPERTY(VisibleAnywhere, Category="Core Logic")
	TEnumAsByte<EFireLogicType> FireLogic;

	/** Weapon skeletal mesh */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asthetic")
	USkeletalMeshComponent* WeaponMesh;
	
	/** Empty shell static mesh */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category="Asthetic")
	TSubclassOf<AActor> EjectClass;

	/** Sound to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acoustic")
	USoundBase* FireSound;

	/** Sound to play when clip is empy */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acoustic")
	USoundBase* EmptySound;

	/** Sound to play when reloading */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Acoustic")
	USoundBase* ReloadSound;

	UPROPERTY()
	float VolumeModifier = 0.3;
	
	UPROPERTY()
	FTimerHandle ReloadTimer;

	UPROPERTY()
	FTimerHandle FireTimer;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asthetic")
	FVector MuzzleOffset;

	UPROPERTY(VisibleAnywhere)
	bool IsActiveWeapon;

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	UDataTable* WeaponDataTable;

	/** Damage from 1 hit */
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int Damage;
	
	/** Current Ammo */
	UPROPERTY(VisibleAnywhere)
	int Ammo;

	/** Clip Size */
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	int ClipSize;

	/** Reload Speed */
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	double ReloadSpeed;

	/** Max Fire Rate */
	UPROPERTY(VisibleAnywhere, Category = "Stats")
	float FireRate;

	/** For firing animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float LastFireTime = -100.0;

	UFUNCTION(BlueprintCallable)
	float GetLastFireTime() { return LastFireTime; }

	/** For reloading animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float LastReloadTime = -100.0;

	UPROPERTY()
	APlayerCharacter* WeaponOwner;

	UFUNCTION(BlueprintCallable)
	void SetWeaponOwner(APlayerCharacter* Player) { this->WeaponOwner = Player; }

	UFUNCTION(BlueprintCallable)
	APlayerCharacter* GetWeaponOwner(APlayerCharacter* Player) { return this->WeaponOwner; }

	UFUNCTION(BlueprintCallable)
	float GetLastReloadTime() { return LastReloadTime; }

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EWeaponType> GetWeaponType() { return WeaponType; }

	UPROPERTY(VisibleAnywhere, Category = "Stats")
	FName WeaponName;

	UFUNCTION()
	void SetupStats(FName RowName);

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void PrimaryFirePressed();

	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void PrimaryFireReleased();

	UFUNCTION(BlueprintCallable, Category="Backend")
	void FireHitScan();
	
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void SecondaryFirePressed();

	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void SecondaryFireReleased();

	/** Reload Weapon */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void Reload();

	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void ApplyReload();

	UFUNCTION()
	virtual void OnEquip();
	
	UFUNCTION()
	virtual void OnUnEquip();
	
};