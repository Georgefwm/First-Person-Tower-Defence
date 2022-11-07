// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "MyProject/Enemies/Enemy.h"
#include "Weapon.generated.h"


UENUM(BlueprintType)
enum EWeaponType
{
	Shotgun UMETA(Displayname, "Shotgun"),
	Rifle UMETA(Displayname, "Rifle"),
	Pistol UMETA(Displayname, "Pistol"),
	Smg UMETA(Displayname, "SMG"),
	BuildTool UMETA(Displayname, "Build Tool")
};

UENUM(BlueprintType)
enum EFireLogicType
{
	Hitscan,
	Projectile
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
	
	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asthetic")
	UAnimationAsset* FireAnimation;

	/** AnimMontage to play each time we fire */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Asthetic")
	UAnimationAsset* ReloadAnimation;

	UPROPERTY()
	FTimerHandle ReloadTimer;

	UPROPERTY()
	FTimerHandle FireTimer;

	/** Gun muzzle's offset from the characters location */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category="Asthetic")
	FVector MuzzleOffset;

	UPROPERTY(VisibleAnywhere)
	bool IsActiveWeapon;

	/** Damage from 1 hit */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	int Damage;
	
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

	/** For firing animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float LastFireTime = -100.0;

	UFUNCTION(BlueprintCallable)
	float GetLastFireTime() { return LastFireTime; }

	/** For reloading animation */
	UPROPERTY(EditAnywhere, BlueprintReadWrite, Category = "Defaults")
	float LastReloadTime = -100.0;

	UFUNCTION(BlueprintCallable)
	float GetLastReloadTime() { return LastReloadTime; }

	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetWeaponMesh() { return WeaponMesh; }

	UFUNCTION(BlueprintCallable)
	TEnumAsByte<EWeaponType> GetWeaponType() { return WeaponType; }

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void PrimaryFirePressed();

	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void PrimaryFireReleased();

	UFUNCTION(BlueprintCallable, Category="Backend")
	void FireHitScan();
	
	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void SecondaryFire();

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