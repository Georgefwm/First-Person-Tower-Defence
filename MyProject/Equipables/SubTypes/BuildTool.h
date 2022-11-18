// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Buildings/Building.h"
#include "MyProject/Equipables/Weapon.h"
#include "BuildTool.generated.h"

UCLASS()
class MYPROJECT_API ABuildTool : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ABuildTool();


	// For the build tool menu
	UPROPERTY(VisibleDefaultsOnly, BlueprintReadWrite, Category = "HUD Elements")
	TSubclassOf<UUserWidget> BuildToolMenuClass;

	UPROPERTY()
	UUserWidget* BuildToolMenuWidget;

	UPROPERTY(EditDefaultsOnly)
	TArray<TSubclassOf<ABuilding>> Buildings;

	UPROPERTY(EditDefaultsOnly, Category = "Stats")
	UDataTable* BuildingDataTable;
	
	UPROPERTY()
	int BuildRotationOffset = 0;

	UPROPERTY(VisibleAnywhere)
	int SelectedBuildingIndex;

	UPROPERTY()
	ABuilding* BuildingBeingPlaced;

	UPROPERTY()
	bool MenuOpen = false;

	UPROPERTY()
	bool CurrentlyPlacing = false;

	UFUNCTION()
	void OpenBuildMenu();

	UFUNCTION()
	void CloseBuildMenu();

	UFUNCTION()
	FVector GetBuildLocation();

	UFUNCTION()
	FRotator GetBuildRotation();

	UFUNCTION(BlueprintCallable)
	void SetSelectedBuilding(int Index);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Make the weapon Fire a Projectile */
	virtual void PrimaryFirePressed() override;
	
	/** Make the weapon Fire a Projectile */
	virtual void SecondaryFire() override;

	/** Reload Weapon */
	virtual void Reload() override;

	virtual void OnEquip() override;
	
	virtual void OnUnEquip() override;
};
