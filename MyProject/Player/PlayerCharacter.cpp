// Copyright Epic Games, Inc. All Rights Reserved.

#include "PlayerCharacter.h"

#include "../MyProjectProjectile.h"
#include "Animation/AnimInstance.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraComponent.h"
#include "Components/CapsuleComponent.h"
#include "Components/InputComponent.h"
#include "MyProject/Equipables/Weapon.h"
#include "GameFramework/InputSettings.h"
#include "Net/UnrealNetwork.h"


//////////////////////////////////////////////////////////////////////////
// APlayerCharacter

APlayerCharacter::APlayerCharacter()
{
	// Set size for collision capsule
	GetCapsuleComponent()->InitCapsuleSize(55.f, 96.0f);
	
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
	
	// Create a mesh component that will be used when being viewed from a '3rd person' view (What other players see)
	GetMesh()->SetOwnerNoSee(true);
	GetMesh()->bCastDynamicShadow = true;
	GetMesh()->CastShadow = true;
	GetMesh()->SetRelativeRotation(FRotator(0.0f, -90.0f, 0.0f));
	GetMesh()->SetRelativeLocation(FVector(0.0f, 0.0f, -95.0f));

	this->CurrentHealthPoints = this->MaxHealthPoints;

	static ConstructorHelpers::FClassFinder<UUserWidget> HUDWidgetFinder(TEXT("/Game/Player/HUD/PlayerHUDBP"));
	HUDClass = HUDWidgetFinder.Class;
	
	if (IsValid(HUDClass))
	{
		HUDWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), HUDClass));

		if (HUDWidget != nullptr)
		{
			HUDWidget->AddToViewport();
		}
	}

	static ConstructorHelpers::FClassFinder<UUserWidget> PauseMenuWidgetFinder(TEXT("/Game/Levels/PauseMenu/PauseMenuWidget"));
	PauseWidgetClass = PauseMenuWidgetFinder.Class;

	if (IsValid(PauseWidgetClass))
	{
		PauseMenuWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), PauseWidgetClass));
	}
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();
	
	if(HasAuthority())
	{
		FActorSpawnParameters SpawnParams;
		SpawnParams.Owner = this;
		
		EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(PrimaryWeaponClass, SpawnParams);
		if (EquippedWeapon)
		{
			EquippedWeapon->SetWeaponOwner(this);
			PreviousWeapon = EquippedWeapon;
			EquippedWeapon->OnEquip();
			Weapons.Add(EquippedWeapon);
			
			EquippedWeapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("GripPoint"));
		}
		
		if (AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(SecondaryWeaponClass, SpawnParams))
		{
			Weapon->SetWeaponOwner(this);
			Weapon->OnUnEquip();
			Weapons.Add(Weapon);

			Weapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("GripPoint"));
		}
		
		if (AWeapon* Weapon = GetWorld()->SpawnActor<AWeapon>(BuildToolClass, SpawnParams))
		{
			Weapon->SetWeaponOwner(this);
			Weapon->OnUnEquip();
			Weapons.Add(Weapon);

			Weapon->AttachToComponent(GetMesh1P(), FAttachmentTransformRules::SnapToTargetNotIncludingScale,
			FName("GripPoint"));
		}
	}
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
	PlayerInputComponent->BindAction("PrimaryAction", IE_Released, this, &APlayerCharacter::OnPrimaryActionReleased);

	// Secondary fire Events
	PlayerInputComponent->BindAction("SecondaryAction", IE_Pressed, this, &APlayerCharacter::OnSecondaryActionPressed);
	PlayerInputComponent->BindAction("SecondaryAction", IE_Released, this, &APlayerCharacter::OnSecondaryActionReleased);

	// Reload Event
	PlayerInputComponent->BindAction("Reload", IE_Pressed, this, &APlayerCharacter::OnReload);

	// Weapon Slot Events
	PlayerInputComponent->BindAction("EquipSlot1", IE_Pressed, this, &APlayerCharacter::OnEquipSlot1Pressed);
	PlayerInputComponent->BindAction("EquipSlot2", IE_Pressed, this, &APlayerCharacter::OnEquipSlot2Pressed);
	PlayerInputComponent->BindAction("EquipSlot3", IE_Pressed, this, &APlayerCharacter::OnEquipSlot3Pressed);
	
	// Bind movement events
	PlayerInputComponent->BindAxis("Move Forward / Backward", this, &APlayerCharacter::MoveForward);
	PlayerInputComponent->BindAxis("Move Right / Left", this, &APlayerCharacter::MoveRight);

	// UI events
	PlayerInputComponent->BindAction("Pause", IE_Pressed, this, &APlayerCharacter::OnPausedPressed);

	// We have 2 versions of the rotation bindings to handle different kinds of devices differently
	// "Mouse" versions handle devices that provide an absolute delta, such as a mouse.
	// "Gamepad" versions are for devices that we choose to treat as a rate of change, such as an analog joystick
	PlayerInputComponent->BindAxis("Turn Right / Left Mouse", this, &APawn::AddControllerYawInput);
	PlayerInputComponent->BindAxis("Look Up / Down Mouse", this, &APawn::AddControllerPitchInput);
	PlayerInputComponent->BindAxis("Turn Right / Left Gamepad", this, &APlayerCharacter::TurnAtRate);
	PlayerInputComponent->BindAxis("Look Up / Down Gamepad", this, &APlayerCharacter::LookUpAtRate);
}

//////////////////////////////////////////////////////////////////////////// HUD-Player

void APlayerCharacter::IncrementGold(int Amount)
{
	Gold += Amount;
}

void APlayerCharacter::DecrementGold(int Amount)
{
	Gold -= Amount;
}

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

int APlayerCharacter::GetClipSize()
{
	return this->EquippedWeapon->ClipSize;
}

int APlayerCharacter::GetCurrentClipCount()
{
	return this->EquippedWeapon->Ammo;
}

int APlayerCharacter::GetCurrentGold()
{
	return this->Gold;
}

//////////////////////////////////////////////////////////////////////////// Weapons

void APlayerCharacter::OnPrimaryActionPressed()
{
	
	if (EquippedWeapon)
		EquippedWeapon->PrimaryFirePressed();
}

void APlayerCharacter::OnPrimaryActionReleased()
{
	if (EquippedWeapon)
		EquippedWeapon->PrimaryFireReleased();
}

void APlayerCharacter::OnSecondaryActionPressed()
{
	if (EquippedWeapon)
		EquippedWeapon->SecondaryFirePressed();
}

void APlayerCharacter::OnSecondaryActionReleased()
{
	
}

void APlayerCharacter::OnReload()
{
	if (EquippedWeapon)
		EquippedWeapon->Reload();
}

void APlayerCharacter::SwitchWeapon(unsigned int Slot)
{
	if (Weapons.IsValidIndex(Slot))
	{
		if (Weapons[Slot] != EquippedWeapon)
		{
			EquippedWeapon->OnUnEquip();
			EquippedWeapon = Weapons[Slot];
			EquippedWeapon->OnEquip();
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("INVALID WEAPON INDEX"));
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