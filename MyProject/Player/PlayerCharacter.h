// Copyright Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "PlayerCharacter.generated.h"


class AWeapon;
class UInputComponent;
class USkeletalMeshComponent;
class USceneComponent;
class UCameraComponent;
class UAnimMontage;
class USoundBase;

// Declaration of the delegate that will be called when the Primary Action is triggered
// It is declared as dynamic so it can be accessed also in Blueprints
DECLARE_DYNAMIC_MULTICAST_DELEGATE(FOnUseItem);

UCLASS(config=Game)
class APlayerCharacter : public ACharacter
{
	GENERATED_BODY()

	

	

public:
	APlayerCharacter();

protected:
	virtual void BeginPlay();

public:
	/** Base turn rate, in deg/sec. Other scaling may affect final turn rate. */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category=Camera)
	float TurnRateGamepad;

	/** Delegate to whom anyone can subscribe to receive this event */
	UPROPERTY(BlueprintAssignable, Category = "Interaction")
	FOnUseItem OnUseItem;
	
//////////////////////////////////////////////////////////////////
/// First person 

	/** Pawn mesh: 1st person view (arms; seen only by self) */
	UPROPERTY(EditDefaultsOnly, Category=Mesh)
	USkeletalMeshComponent* Mesh1P;

	/** First person camera */
	UPROPERTY(VisibleAnywhere, BlueprintReadOnly, Category = Camera, meta = (AllowPrivateAccess = "true"))
	UCameraComponent* FirstPersonCameraComponent;

	/** Returns Mesh1P subobject **/
	UFUNCTION(BlueprintCallable)
	USkeletalMeshComponent* GetMesh1P() const { return Mesh1P; }
	
	/** Returns FirstPersonCameraComponent subobject **/
	UCameraComponent* GetFirstPersonCameraComponent() const { return FirstPersonCameraComponent; }
	
//////////////////////////////////////////////////////////////////

	
//////////////////////////////////////////////////////////////////
/// Weapon system

	/** Weapon Slots */
	TArray<AWeapon*> Weapons;

	////// Weapon Classes
	
	/** Weapon class to use for primary weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon System")
	TSubclassOf<AWeapon> PrimaryWeaponClass;

	/** Weapon class to use for primary weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon System")
	TSubclassOf<AWeapon> SecondaryWeaponClass;

	/** Weapon class to use for primary weapon */
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "Weapon System")
	TSubclassOf<AWeapon> BuildToolClass;

	/** Currently equipped weapon slot */
	UPROPERTY(EditAnywhere, Category = "Weapon System")
	AWeapon* EquippedWeapon;

	////// Weapon Functions
	
	UFUNCTION()
	void SetupWeapons();

	/**
	 * @brief Handles weapon switching. Shows/hides models, and sets active weapon.
	 * @param Slot : Set the index of the weapon array to be active at this time
	 */
	UFUNCTION()
	void SwitchWeapon(unsigned int Slot);

	UFUNCTION()
	void OnReload();

	UFUNCTION()
	void OnEquipSlot1Pressed();
	void OnEquipSlot2Pressed();
	void OnEquipSlot3Pressed();

	UFUNCTION(BlueprintCallable)
	AWeapon* GetEquippedWeapon() { return EquippedWeapon; }

	UFUNCTION(BlueprintCallable)
	AWeapon* GetBuildTool() { return Weapons[2]; }

//////////////////////////////////////////////////////////////////


//////////////////////////////////////////////////////////////////
/// HUD-Player system

/** fields */
private:
	/** Maximum Health */
	UPROPERTY(EditAnywhere, Category = "HUD Elements")
	int MaxHealthPoints = 100;

	/** Current Health */
	UPROPERTY(EditAnywhere, Category = "HUD Elements")
	int CurrentHealthPoints;

	/** Maximum Resource Amount */
	UPROPERTY(EditAnywhere, Category = "HUD Elements")
	int MaxMetal;

	/** Resource Amount */
	UPROPERTY(EditAnywhere, Category = "HUD Elements")
	int CurrentMetal;

protected:
	UPROPERTY(EditDefaultsOnly, BlueprintReadWrite, Category = "HUD Elements")
	TSubclassOf<UUserWidget> HUDClass;

	UPROPERTY()
	UUserWidget* CurrentWidget;

/** Functions */
public:
	UFUNCTION(BlueprintCallable, Category = "HUD Elements")
	void UpdateHealthPoints(int Mod);
	
	UFUNCTION(BlueprintCallable, Category = "HUD Elements")
	int GetMaxHealthPoints();
	
	UFUNCTION(BlueprintCallable, Category = "HUD Elements")
	int GetCurrentHealthPoints();

	UFUNCTION(BlueprintCallable, Category = "HUD Elements")
	int GetCurrentMetal();
	
//////////////////////////////////////////////////////////////////
	
protected:
	
	/** Fire or build */
	void OnPrimaryActionPressed();
	void OnPrimaryActionReleased();

	/** Alt fire */
	void OnSecondaryActionPressed();
	void OnSecondaryActionReleased();

	/** Handles moving forward/backward */
	void MoveForward(float Val);

	/** Handles strafing movement, left and right */
	void MoveRight(float Val);

	/**
	 * Called via input to turn at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void TurnAtRate(float Rate);

	/**
	 * Called via input to turn look up/down at a given rate.
	 * @param Rate	This is a normalized rate, i.e. 1.0 means 100% of desired turn rate
	 */
	void LookUpAtRate(float Rate);

	void LineTrace(FHitResult& HitRes, double Distance);

	struct TouchData
	{
		TouchData() { bIsPressed = false;Location=FVector::ZeroVector;}
		bool bIsPressed;
		ETouchIndex::Type FingerIndex;
		FVector Location;
		bool bMoved;
	};
	
	void BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location);
	void TouchUpdate(const ETouchIndex::Type FingerIndex, const FVector Location);
	TouchData	TouchItem;
	
protected:
	// APawn interface
	virtual void SetupPlayerInputComponent(UInputComponent* InputComponent) override;
	// End of APawn interface
	
};

