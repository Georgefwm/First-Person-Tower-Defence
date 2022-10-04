// Fill out your copyright notice in the Description page of Project Settings.


#include "BuildTool.h"
#include "GameFramework/PlayerController.h"
#include "Blueprint/UserWidget.h"


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
}

void ABuildTool::OpenBuildMenu()
{
	// Get cleared occasionally
	if (BuildToolMenuWidget != nullptr)
		BuildToolMenuWidget = Cast<UUserWidget>(CreateWidget(GetWorld(), BuildToolMenuClass));
	
	if (BuildToolMenuWidget != nullptr)
	{
		BuildToolMenuWidget->AddToViewport();
		MenuOpen = true;
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
		BuildToolMenuWidget->RemoveFromViewport();
		MenuOpen = false;
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

