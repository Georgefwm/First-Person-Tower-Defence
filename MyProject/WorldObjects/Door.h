// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "Door.generated.h"

UCLASS()
class MYPROJECT_API ADoor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ADoor();

	UPROPERTY()
	float OriginalDoorOffset;

	UPROPERTY()
	bool ShouldBeOpen = false;

	UPROPERTY(EditAnywhere, Category = "Door Properties")
	float MaxOpenOffset = 110.0;

	// Can maybe turn this into curve?
	UPROPERTY(EditAnywhere, Category = "Door Properties")
	float OpenSpeed = 225.0;

	UPROPERTY(EditAnywhere, Category = "Door Properties")
	UCurveFloat* OpenCurve;

	UPROPERTY(EditAnywhere, Category = "Door Properties")
	bool Locked = false;

	UFUNCTION()
	void UpdateDoorPosition(float DeltaTime);

	UFUNCTION()
	bool ShouldUpdateThisTick();

	UFUNCTION()
	float GetOpenPercentage();

	UFUNCTION()
	void SetLocked(bool NewLockStatus);

protected:
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UPROPERTY()
	USceneComponent* SceneRoot;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Base;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorLeft;

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* DoorRight;

	UPROPERTY(EditAnywhere)
	UBoxComponent* OverlapComponent;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
