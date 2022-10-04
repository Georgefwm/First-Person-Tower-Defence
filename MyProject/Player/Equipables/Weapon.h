// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "../PlayerCharacter.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"


/**
 * 
 */
UCLASS()
class MYPROJECT_API AWeapon : public AActor
{
	GENERATED_BODY()

public:
	/** Sets default values for this component's properties */
	AWeapon();

	void BeginPlay() override;
	

	/** Sound to play each time we fire */
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

	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void PrimaryFire();
	
	/** Make the weapon Fire a Projectile */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void SecondaryFire();

	/** Reload Weapon */
	UFUNCTION(BlueprintCallable, Category="Backend")
	virtual void Reload();

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
	USkeletalMeshComponent* GetWeaponMesh() { return Mesh; }

	
public:
	/** The Character holding this weapon*/
	USkeletalMeshComponent* Mesh;

	UFUNCTION()
	virtual void OnEquip();

	UFUNCTION()
	virtual void OnUnEquip();
};