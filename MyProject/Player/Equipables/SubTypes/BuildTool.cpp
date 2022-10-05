// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildTool.h"

#include "VectorUtil.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"
#include "Camera/CameraActor.h"
#include "Camera/CameraComponent.h"


// Sets default values
ABuildTool::ABuildTool()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	static ConstructorHelpers::FClassFinder<UUserWidget> PlayerPawnClassFinder(TEXT("/Game/Player/BuildTool/BuildMenuWidget"));
	BuildToolMenuClass = PlayerPawnClassFinder.Class;
	
	if (BuildToolMenuClass)
		BuildToolMenuWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), BuildToolMenuClass));
	
	if (BuildToolMenuWidget != nullptr)
	{
		FVector2d MenuSize = { 200, 200 };
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
		APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetAttachParentActor());
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

void ABuildTool::PrimaryFire()
{
	Super::PrimaryFire();

	if (!MenuOpen && this->IsActiveWeapon)
	{
		APlayerCharacter* OwningPlayer = Cast<APlayerCharacter>(GetAttachParentActor());
		if (OwningPlayer == nullptr)
			return;

		GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, TEXT("Looking for building placement"));

		FMinimalViewInfo CameraView;
		OwningPlayer->CalcCamera(GetWorld()->DeltaTimeSeconds, CameraView);

		// Clamp Y axis rotation so the player can't spawn buildings directly above/below them
		UE::Math::TVector<double> ClampedCameraRotation = CameraView.Rotation.Vector();
		ClampedCameraRotation.Y = UE::Geometry::VectorUtil::Clamp(ClampedCameraRotation.Y, -40.0, 40.0);
		
		FVector Location = CameraView.Location + ClampedCameraRotation * 200;
		FVector Down = { 0.0, 0.0, -1.0 };

		FVector From = Location;
		FVector To = Location + Down * 200;

		FHitResult HitRes;
		
		FCollisionQueryParams TraceParams;
		TraceParams.AddIgnoredActor(this);
		
		if (GetWorld()->LineTraceSingleByChannel(HitRes, From, To, ECC_Visibility, TraceParams))
		{
			DrawDebugLine(GetWorld(), From, HitRes.ImpactPoint, FColor(255, 0, 0), true, -1);
			DrawDebugBox(GetWorld(), HitRes.ImpactPoint, FVector(5, 5, 5), FColor::Emerald, true, -1);
			DrawDebugLine(GetWorld(), HitRes.ImpactPoint, To, FColor(255, 255, 0), true, 1);

			GEngine->AddOnScreenDebugMessage(-1, 3.0f, FColor::Yellow, HitRes.GetActor()->GetActorNameOrLabel());

			Location = HitRes.ImpactPoint;

			// We only want the building to face the direction the player is facing (for now)
			double DesiredYaw = GetActorForwardVector().ToOrientationRotator().Yaw;
			FRotator Rotation = { 0, DesiredYaw, 0 };
			
			FActorSpawnParameters SpawnInfo;
			// No janky spawning locations please :)
			SpawnInfo.SpawnCollisionHandlingOverride = ESpawnActorCollisionHandlingMethod::AdjustIfPossibleButDontSpawnIfColliding;
			
			GetWorld()->SpawnActor<ABuilding>(Buildings[SelectedBuildingIndex], Location, Rotation, SpawnInfo);
		}
	}
}

void ABuildTool::SecondaryFire()
{
	Super::SecondaryFire();
}

void ABuildTool::Reload()
{
	Super::Reload();
	
	if (MenuOpen) CloseBuildMenu();
	else OpenBuildMenu();
	
}

void ABuildTool::OnEquip()
{
	Super::OnEquip();
	// controllerRef->SetInputMode(FInputModeGameOnly())
}

void ABuildTool::OnUnEquip()
{
	Super::OnUnEquip();
	
	if (MenuOpen)
		CloseBuildMenu();
}

void ABuildTool::SetSelectedBuilding(int Index)
{
	if (IsValid(Buildings[Index]))
	{
		SelectedBuildingIndex = Index;
	}
}

