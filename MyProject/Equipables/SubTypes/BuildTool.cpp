// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildTool.h"

#include "VectorUtil.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"
#include "Kismet/KismetMathLibrary.h"
#include "Toolkits/IToolkit.h"


// Sets default values
ABuildTool::ABuildTool()
{
	WeaponType = EWeaponType::BuildTool;
	
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerPawnClassFinder(TEXT("/Game/Player/BuildTool/BuildMenuWidget"));
	BuildToolMenuClass = PlayerPawnClassFinder.Class;
	
	if (BuildToolMenuClass)
		BuildToolMenuWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), BuildToolMenuClass));
	
	if (BuildToolMenuWidget != nullptr)
	{
		FVector2d MenuSize = { 600, 400 };
		BuildToolMenuWidget->SetDesiredSizeInViewport(MenuSize);
		MenuOpen = false;
	}

	static ConstructorHelpers::FClassFinder<ABuilding> BuildingClassFinder(TEXT("/Game/Buildings/BasicBuilding/BasicBuildingBP"));
	Buildings.Add(BuildingClassFinder.Class);

	SelectedBuildingIndex = 0;
}

void ABuildTool::OpenBuildMenu()
{
	// Get cleared occasionally
	if (BuildToolMenuWidget != nullptr)
		BuildToolMenuWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), BuildToolMenuClass));
	
	if (BuildToolMenuWidget != nullptr)
	{
		APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetAttachParentActor());
		if (OwningPlayer != nullptr)
		{
			OwningPlayer->GetLocalViewingPlayerController()->bShowMouseCursor = true;
			OwningPlayer->GetLocalViewingPlayerController()->bEnableClickEvents = true;
			OwningPlayer->GetLocalViewingPlayerController()->ClientIgnoreLookInput(true);
			
			BuildToolMenuWidget->AddToViewport();
			MenuOpen = true;
		}
		
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Widget is a nullptr"));
	}
	
}

void ABuildTool::CloseBuildMenu()
{
	if (BuildToolMenuWidget != nullptr)
	{
		APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetOwner());
		if (OwningPlayer != nullptr)
		{
			OwningPlayer->GetLocalViewingPlayerController()->bShowMouseCursor = false;
			OwningPlayer->GetLocalViewingPlayerController()->bEnableClickEvents = false;
			OwningPlayer->GetLocalViewingPlayerController()->ClientIgnoreLookInput(false);
			
			BuildToolMenuWidget->RemoveFromViewport();
			MenuOpen = false;
		}
	}
	else
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Widget is a nullptr"));
	}
}

FVector ABuildTool::GetBuildLocation()
{
	if (WeaponOwner == nullptr)
	{
		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Red,
			TEXT("Build tool does not have an owner"));
	
		return FVector(0.0, 0.0, 0.0);
	}

	FMinimalViewInfo CameraView;
	WeaponOwner->CalcCamera(GetWorld()->DeltaTimeSeconds, CameraView);
	
	// Clamp Y axis rotation so the player can't spawn buildings directly above/below them
	UE::Math::TVector<double> ClampedCameraRotation = CameraView.Rotation.Vector();
	ClampedCameraRotation.Z = UE::Geometry::VectorUtil::Clamp(ClampedCameraRotation.Z, -0.5, 0.5);
	FVector CameraViewFlatForwardVector = CameraView.Location + ClampedCameraRotation;

	double ForwardBuildDistance = 150;

	// EW! Maths
	// Calculate the distance that the building should spawn regardless of the camera pitch
	double PitchInRadians = UKismetMathLibrary::DegreesToRadians(CameraView.Rotation.Pitch);
	double ClampedPitchInRadians = UE::Geometry::VectorUtil::Clamp(PitchInRadians, -45.0, 45.0);
	float DesiredDistance = ForwardBuildDistance / UKismetMathLibrary::Cos(ClampedPitchInRadians);

	FVector Location = CameraView.Location + ClampedCameraRotation * DesiredDistance;
	FVector Down = { 0.0, 0.0, -1.0 };

	FVector From = Location;
	FVector To = Location + Down * 250;

	//DrawDebugLine(GetWorld(), CameraViewFlatForwardVector, From, FColor(255, 0, 0), true, -1);

	FHitResult HitRes;
	
	FCollisionQueryParams TraceParams;
	TraceParams.AddIgnoredActor(this);
	
	if (GetWorld()->LineTraceSingleByChannel(HitRes, From, To, ECC_Visibility, TraceParams))
	{
		// DrawDebugLine(GetWorld(), From, HitRes.ImpactPoint, FColor(255, 0, 0), true, -1);
		// DrawDebugBox(GetWorld(), HitRes.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, true, -1);
		// DrawDebugLine(GetWorld(), HitRes.ImpactPoint, To, FColor(255, 255, 0), true, 1);
	
		return HitRes.ImpactPoint;
	}

	return FVector(0.0, 0.0, 0.0);
}

FRotator ABuildTool::GetBuildRotation()
{
	FRotator Rotation = { 0, WeaponOwner->GetActorForwardVector().ToOrientationRotator().Yaw, 0 };
	double Change = (static_cast<int>(Rotation.Yaw) + BuildRotationOffset) % 360;
	Rotation.Yaw = Change;
	
	return Rotation;
}

// Called when the game starts or when spawned
void ABuildTool::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ABuildTool::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

void ABuildTool::PrimaryFirePressed()
{
	if (!MenuOpen && this->IsActiveWeapon)
	{
		FVector Location = GetBuildLocation();
		FRotator Rotation = GetBuildRotation();
		
		if (Location == FVector(0.0, 0.0, 0.0))
		{
			return;
		}

		ABuilding* NewBuilding = GetWorld()->SpawnActor<ABuilding>(Buildings[SelectedBuildingIndex], Location, Rotation);
		if (NewBuilding)
		{
			NewBuilding->SetBuildingOwner(WeaponOwner);
			NewBuilding->SetBuildingState(EBuildingState::BS_Placing);
		}
	}
}

// Adds a rotation offset to the build location
void ABuildTool::SecondaryFire()
{
	int Change = (BuildRotationOffset + 90) % 360;
	BuildRotationOffset = Change;
}

void ABuildTool::Reload()
{
	if (MenuOpen)
		CloseBuildMenu();
	else
		OpenBuildMenu();
	
}

void ABuildTool::OnEquip()
{
	Super::OnEquip();

	BuildRotationOffset = 0;
}

void ABuildTool::OnUnEquip()
{
	Super::OnUnEquip();
	
	BuildRotationOffset = 0;
	
	if (MenuOpen)
	{
		CloseBuildMenu();
	}
}

void ABuildTool::SetSelectedBuilding(int Index)
{
	if (IsValid(Buildings[Index]))
	{
		SelectedBuildingIndex = Index;
	}
}

