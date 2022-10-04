// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "MyProject/Buildings/Building.h"
#include "MyProject/Player/Equipables/Weapon.h"
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

	UPROPERTY(VisibleAnywhere)
	TArray<TSubclassOf<ABuilding>> Buildings;

	UPROPERTY(VisibleAnywhere)
	int SelectedBuildingIndex;

	UPROPERTY()
	bool MenuOpen = false;

	UFUNCTION()
	void OpenBuildMenu();

	UFUNCTION()
	void CloseBuildMenu();

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	/** Make the weapon Fire a Projectile */
	virtual void PrimaryFire() override;
	
	/** Make the weapon Fire a Projectile */
	virtual void SecondaryFire() override;

	/** Reload Weapon */
	virtual void Reload() override;

	virtual void OnEquip() override;
	
	virtual void OnUnEquip() override;

	UFUNCTION(BlueprintCallable)
	void SetSelectedBuilding(int Index);
};
