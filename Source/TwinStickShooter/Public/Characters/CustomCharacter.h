// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon.h"
#include "Components/TimelineComponent.h"
#include "GameFramework/Character.h"
#include "CustomCharacter.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API ACustomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomCharacter();

protected:

	// ------ WEAPONS ------

	UPROPERTY(EditAnywhere, Category="Weapons")
	int32 MaxWeaponCapacity = 2;

	UPROPERTY(EditAnywhere, Category="Weapons")
	TSet<TSubclassOf<AWeapon>> StartingWeapons;
	
	UPROPERTY()
	TArray<AWeapon*> Weapons;
	

	UPROPERTY()
	AWeapon* EquippedWeapon = nullptr;

	// ------ DASH ------

	// This timer will stop the dash once it has reached the time it's meant to stop.
	UPROPERTY()
	FTimerHandle DashTimer;

	// This timer is what keeps the dash going on loop.
	UPROPERTY()
	FTimerHandle DashUpdateTimer;

	// This timer is how long the character has to wait before begin able to dash after a previous dash.
	UPROPERTY()
	FTimerHandle DashCooldownTimer;

	// How long the dash will last.
	UPROPERTY(EditAnywhere, Category="Movement")
	float DashTime = 0.2f;

	// How fast the character will move during the dash period.
	UPROPERTY(EditAnywhere, Category="Movement")
	float DashSpeed = 3000.0f;

	UPROPERTY(EditAnywhere, Category="Movement")
	float DashCooldown = 0.1f;

	// UPROPERTY(EditAnywhere, Category="Movement")
	// float DashStaminaCost = 33.33f;

	// ----- CHARACTER VARIABLES ------

	UPROPERTY(EditAnywhere, Category="Custom Character")
	float MaxHealth = 100;
	
	UPROPERTY()
	float CurrentHealth = 0;

	// UPROPERTY(EditAnywhere, Category="Custom Character")
	// float MaxStamina = 100;
	//
	// UPROPERTY()
	// float CurrentStamina = 0;

	// ------ FUNCTIONS ------
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

public:

	UFUNCTION(BlueprintCallable)
	void Dash();

	UFUNCTION()
	void UpdateDash(FVector DashDirectionAndSpeed);

	UFUNCTION()
	void StopDash();

	UFUNCTION(BlueprintCallable)
	void UseWeapon();

	UFUNCTION(BlueprintCallable)
	void StopUsingWeapon();

	UFUNCTION(BlueprintCallable)
	void ReloadWeapon();

	UFUNCTION(BlueprintCallable)
	AWeapon* SpawnWeapon(TSubclassOf<AWeapon> WeaponToSpawn);

	UFUNCTION(BlueprintCallable)
	void EquipNextWeapon();

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(AWeapon* NewWeapon);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTimerHandle GetDashTimer() { return DashTimer; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	AWeapon* GetEquippedWeapon() { return EquippedWeapon; }
};
