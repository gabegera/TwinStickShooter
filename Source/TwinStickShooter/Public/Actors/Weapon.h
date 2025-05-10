// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "Weapon.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	UPROPERTY(EditAnywhere)
	UArrowComponent* Arrow;

	UPROPERTY(EditAnywhere, Category="Weapon")
	int32 Ammo;

	UPROPERTY(EditAnywhere, Category="Weapon")
	int32 MagazineCapacity;

	UPROPERTY()
	int32 AmmoInMagazine;

	// The Projectile that this weapon fires.
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AProjectile> Projectile;

	UPROPERTY(EditAnywhere, Category="Weapon")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, Category="Weapon")
	float ProjectileSpeed = 1000.0f;

	UPROPERTY(EditAnywhere, Category="Weapon")
	bool isAutomatic = false;

	UPROPERTY()
	FTimerHandle FireRateTimer;

	// The weapons fire rate in Rounds Per Minute;
	UPROPERTY(EditAnywhere, Category="Weapon", meta=(EditCondition="isAutomatic"))
	float FireRateRpm;

	UPROPERTY()
	bool isTriggerPulled = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Triggers the weapons attack.
	UFUNCTION(BlueprintCallable, BlueprintNativeEvent)
	void Fire();

	UFUNCTION()
	void ReleaseFire();

	UFUNCTION(BlueprintCallable)
	void ShootProjectile();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ MATH ------

	UFUNCTION(BlueprintCallable)
	float RPMToSeconds(float FireRate) { return 1 / (FireRate / 60); }


	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USkeletalMeshComponent* GetSkeletalMesh() const { return SkeletalMesh; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UArrowComponent* GetArrowComponent() const { return Arrow; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetAmmoInMagazine() const { return AmmoInMagazine; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetMagazineCapacity() const { return MagazineCapacity; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetAmmo() const { return Ammo + AmmoInMagazine;}
};
