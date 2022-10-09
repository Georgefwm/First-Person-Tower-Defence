// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"

#include "../MyProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Engine/Engine.h"
#include "Components/InputComponent.h"
#include "Equipables/Weapon.h"
#include "GameFramework/InputSettings.h"


//////////////////////////////////////////////////////////////////////////
// APlayerCharacter

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);


	//// Client ////
	
	// set our turn rates for input
	TurnRateGamepad = 45.f;

	// Create a CameraComponent	
	FirstPersonCameraComponent = CreateDefaultSubobject<UCameraComponent>(TEXT("FirstPersonCamera"));
	FirstPersonCameraComponent->SetupAttachment(GetCapsuleComponent());
	FirstPersonCameraComponent->SetRelativeLocation(FVector(0.0f, 0.0f, 64.f)); // Position the camera
	FirstPersonCameraComponent->bUsePawnControlRotation = true;

	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	Mesh1P = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("CharacterMesh1P"));
	Mesh1P->SetOnlyOwnerSee(true);
	Mesh1P->SetupAttachment(FirstPersonCameraComponent);
	Mesh1P->bCastDynamicShadow = false;
	Mesh1P->CastShadow = false;
	Mesh1P->SetRelativeRotation(FRotator(7.3f, -23.19f, 5.02f));
	Mesh1P->SetRelativeLocation(FVector(26.67f, 6.31f, -158.7f));

	//// Server ////
	bReplicates = true;
	
	// Create a mesh component that will be used when being viewed from a '1st person' view (when controlling this pawn)
	USkeletalMeshComponent* Mesh3P = this->GetMesh();
	Mesh3P->SetOwnerNoSee(true);
	Mesh3P->SetupAttachment(FirstPersonCameraComponent);
	Mesh3P->bCastDynamicShadow = true;
	Mesh3P->CastShadow = true;
	Mesh3P->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	Mesh3P->SetRelativeLocation(FVector(0.0f, 0.0f, -95.0f));

	this->CurrentHealthPoints = this->MaxHealthPoints;
	this->CurrentMetal = this->MaxMetal;

	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerPawnClassFinder(TEXT("/Game/Player/HUD/PlayerHUDBP"));
	HUDClass = PlayerPawnClassFinder.Class;
	
	if (IsValid(HUDClass))
	{
		CurrentWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), HUDClass));

		if (CurrentWidget != nullptr)
		{
			CurrentWidget->AddToViewport();
		}
	}
	
}

void APlayerCharacter::BeginPlay()
{
	// Call the base class  
	Super::BeginPlay();
    
    if (Mesh1P != nullptr)
	   SetupWeapons();
}

//////////////////////////////////////////////////////////////////////////// Input

void APlayerCharacter::SetupPlayerInputComponent(class UInputComponent* PlayerInputComponent)
{
	// Set up gameplay key bindings
	check(PlayerInputComponent);

	// Bind jump events
	PlayerInputComponent->BindAction("Jump", IE_Pressed, this, &ACharacter::Jump);
	PlayerInputComponent->BindAction("Jump", IE_Released, this, &ACharacter::StopJumping);

	// Primary fire Events
	PlayerInputComponent->BindAction("PrimaryAction", IE_Pressed, this, &APlayerCharacter::OnPrimaryActionPressed);
	//PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &APlayerCharacter::OnPrimaryActionReleased);

	// Secondary fire Events
	//PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &APlayerCharacter::OnSecondaryActionPressed);
	//PlayerInputComponent->BindAction("SecondaryAction", IE_Released, this, &APlayerCharacter::OnSecondaryActionReleased);

	// Reload Event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::OnReload);

	// Weapon Slot Events
	PlayerInputComponent->BindAction("EquipSlot1", IE_Pressed, this, &APlayerCharacter::OnEquipSlot1Pressed);
	PlayerInputComponent->BindAction("EquipSlot2", IE_Pressed, this, &APlayerCharacter::OnEquipSlot2Pressed);
	PlayerInputComponent->BindAction("EquipSlot3", IE_Pressed, this, &APlayerCharacter::OnEquipSlot3Pressed);
	
	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &APlayerCharacter::LookUpAtRate);
}

//////////////////////////////////////////////////////////////////////////// HUD-Player

void APlayerCharacter::UpdateHealthPoints(int Mod)
{
	this->CurrentHealthPoints += Mod;

	if (this->CurrentHealthPoints <= 0)
		this->CurrentHealthPoints = 0;
}

int APlayerCharacter::GetMaxHealthPoints()
{
	return this->MaxHealthPoints;
}

int APlayerCharacter::GetCurrentHealthPoints()
{
	return this->CurrentHealthPoints;
}

int APlayerCharacter::GetCurrentMetal()
{
	return this->CurrentMetal;
}

//////////////////////////////////////////////////////////////////////////// Weapons

void APlayerCharacter::OnPrimaryActionPressed()
{
	
	if (IsValid(EquippedWeapon))
		EquippedWeapon->PrimaryFire();
}

void APlayerCharacter::OnPrimaryActionReleased()
{
	
}

void APlayerCharacter::OnSecondaryActionPressed()
{
	// if (IsValid(EquippedWeapon))
	// 	EquippedWeapon->SecondaryFire();
}

void APlayerCharacter::OnSecondaryActionReleased()
{
	
}

void APlayerCharacter::OnReload()
{
	if (IsValid(EquippedWeapon))
		EquippedWeapon->Reload();
}

void APlayerCharacter::SetupWeapons()
{
	EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass);
	if (EquippedWeapon)
	{
		EquippedWeapon->SetActorHiddenInGame(false);
		EquippedWeapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
		EquippedWeapon->IsActiveWeapon = true;
		
		Weapons.Add(EquippedWeapon);
	}
		
	if (AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass))
	{
		Weapon->SetActorHiddenInGame(true);
		Weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
		Weapon->IsActiveWeapon = false;
		
		Weapons.Add(Weapon);
	}
	
	if (AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(BuildToolClass))
	{
		Weapons.Add(Weapon);
		Weapon->SetActorHiddenInGame(true);
		Weapon->AttachToComponent(Mesh1P, FAttachmentTransformRules::SnapToTargetNotIncludingScale, FName("GripPoint"));
		Weapon->IsActiveWeapon = false;

		Weapons.Add(EquippedWeapon);
	}
	
}

void APlayerCharacter::SwitchWeapon(unsigned int Slot)
{
	if (Weapons[Slot] != EquippedWeapon)
	{
		EquippedWeapon->OnUnEquip();
		EquippedWeapon = Weapons[Slot];
		EquippedWeapon->OnEquip();
	}
}

void APlayerCharacter::OnEquipSlot1Pressed()
{
	SwitchWeapon(0);
}

void APlayerCharacter::OnEquipSlot2Pressed()
{
	SwitchWeapon(1);
}

void APlayerCharacter::OnEquipSlot3Pressed()
{
	SwitchWeapon(2);
}

void APlayerCharacter::BeginTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == true)
	{
		return;
	}
	if ((FingerIndex == TouchItem.FingerIndex) && (TouchItem.bMoved == false))
	{
		OnPrimaryActionPressed();
	}
	TouchItem.bIsPressed = true;
	TouchItem.FingerIndex = FingerIndex;
	TouchItem.Location = Location;
	TouchItem.bMoved = false;
}

void APlayerCharacter::EndTouch(const ETouchIndex::Type FingerIndex, const FVector Location)
{
	if (TouchItem.bIsPressed == false)
	{
		return;
	}
	TouchItem.bIsPressed = false;
}

//////////////////////////////////////////////////////////////////////////// Movement

void APlayerCharacter::MoveForward(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorForwardVector(), Value);
	}
}

void APlayerCharacter::MoveRight(float Value)
{
	if (Value != 0.0f)
	{
		// add movement in that direction
		AddMovementInput(GetActorRightVector(), Value);
	}
}

void APlayerCharacter::TurnAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerYawInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LookUpAtRate(float Rate)
{
	// calculate delta for this frame from the rate information
	AddControllerPitchInput(Rate * TurnRateGamepad * GetWorld()->GetDeltaSeconds());
}

void APlayerCharacter::LineTrace(FHitResult& HitRes, double Distance)
{
	FVector Location;
	FRotator Rotation;
	
	GetController()->GetPlayerViewPoint(Location, Rotation);

	FVector From = Location;
	FVector To = From + (Rotation.Vector() * Distance);

	FCollisionQueryParams TraceParms;
	GetWorld()->LineTraceSingleByChannel(HitRes, From, To, ECC_Visibility, TraceParms);
}