// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/RangedWeapon.h"
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

	UPROPERTY(EditAnywhere)
	USphereComponent* SphereCollider;

	UPROPERTY()
	AActor* NearestInteractable;

	// ------ ROTATION ------

	// How fast the character can rotate in degrees per second.
	UPROPERTY(EditAnywhere, Category="Rotation")
	float MaxRotationSpeed = 1080.0f;
	
	FRotator TargetRotation = FRotator::ZeroRotator;

	// Rotates To Target Location at specified speed in the shortest rotation possible.
	UFUNCTION(BlueprintCallable)
	void UpdateRotation();

	// ------ WEAPONS ------

	UPROPERTY(EditAnywhere, Category="RangedWeapons")
	int32 MaxWeaponCapacity = 2;

	UPROPERTY(EditAnywhere, Category="RangedWeapons")
	TSet<TSubclassOf<ARangedWeapon>> StartingRangedWeapons;
	
	UPROPERTY()
	TArray<ARangedWeapon*> RangedWeapons;

	UPROPERTY()
	ARangedWeapon* EquippedRangedWeapon = nullptr;

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
	UPROPERTY(EditAnywhere, Category="Movement|Dash")
	float DashTime = 0.2f;

	// How fast the character will move during the dash period.
	UPROPERTY(EditAnywhere, Category="Movement|Dash")
	float DashSpeed = 3000.0f;

	UPROPERTY(EditAnywhere, Category="Movement|Dash")
	float DashCooldown = 0.1f;
	
	// ----- CHARACTER VARIABLES ------

	UPROPERTY(EditAnywhere, Category="Custom Character")
	float MaxHealth = 100;
	
	UPROPERTY()
	float CurrentHealth = 0;

	// ------ FUNCTIONS ------
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	virtual void OnConstruction(const FTransform& Transform) override;

	UFUNCTION()
	void OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
	void OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex);

public:

	UFUNCTION(BlueprintCallable)
	void Dash(FVector DashDirection);

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

	// Will spawn a new weapon and attach it to the character.
	UFUNCTION(BlueprintCallable)
	ARangedWeapon* SpawnRangedWeapon(TSubclassOf<ARangedWeapon> WeaponToSpawn);

	// Equips the next weapon available in the RangedWeapons Array.
	UFUNCTION(BlueprintCallable)
	void EquipNextWeapon();
	
	UFUNCTION(BlueprintCallable)
	void EquipWeapon(ARangedWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable)
	void PickupWeapon(ARangedWeapon* NewWeapon);

	UFUNCTION(BlueprintCallable)
	void Interact();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FTimerHandle GetDashTimer() { return DashTimer; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	ARangedWeapon* GetEquippedRangedWeapon() { return EquippedRangedWeapon; }

	// ------ SETTERS ------ //

	UFUNCTION(BlueprintCallable)
	FRotator SetTargetRotation(const FRotator NewTarget) { return TargetRotation = NewTarget; }
};
