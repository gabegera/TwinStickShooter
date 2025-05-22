// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "RangedWeapon.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API ARangedWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	ARangedWeapon();

protected:

	UPROPERTY(EditAnywhere)
	UArrowComponent* Arrow;

	// ------ AMMO ------

	// How much ammo is available for a reload.
	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 Ammo;

	// How much ammo can be carried at one time.
	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 MaxAmmo;

	// How much ammo the magazine can hold.
	UPROPERTY(EditAnywhere, Category="Ammo")
	int32 MagazineCapacity;

	// How much ammo is currently in the magazine.
	UPROPERTY()
	int32 AmmoInMagazine;

	// How long it takes to reload the weapon.
	UPROPERTY(EditAnywhere, Category="Ammo")
	float ReloadTime = 1.0f;
	
	UPROPERTY()
	FTimerHandle ReloadTimer;

	// ------ RANGED WEAPON VARIABLES ------

	// The Projectile that this weapon fires.
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AProjectile> Projectile;

	UPROPERTY(EditAnywhere, Category="Weapon")
	float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, Category="Weapon")
	float ProjectileSpread = 2.0f;

	// How many projectiles are spawned when the weapon is fired.
	UPROPERTY(EditAnywhere, Category="Weapon", meta=(UIMin=1, ClampMin=1))
	int32 ProjectileCount = 1;

	UPROPERTY(EditAnywhere, Category="Weapon")
	bool isAutomatic = false;

	// The weapons fire rate in Rounds Per Minute.
	UPROPERTY(EditAnywhere, Category="Weapon", meta=(EditCondition="isAutomatic"))
	float FireRateRpm;
	UPROPERTY() FTimerHandle FireRateTimer;
	
	// How long it takes to pull the trigger again after it is released.
	UPROPERTY(EditAnywhere, Category="Weapon")
	float TriggerDelay = 0.05f;
	UPROPERTY() FTimerHandle TriggerDelayTimer;

	UPROPERTY()
	bool isTriggerPulled = false;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	
	virtual void Fire() override;

	virtual void ReleaseFire() override;

	UFUNCTION(BlueprintCallable)
	void ShootProjectile();

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void CancelReload();

	UFUNCTION()
	void FinishReload();

	// ------ MATH ------

	UFUNCTION(BlueprintCallable)
	float RPMToSeconds(float FireRate) { return 1 / (FireRate / 60); }
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UArrowComponent* GetArrowComponent() const { return Arrow; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetAmmoInMagazine() const { return AmmoInMagazine; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetMagazineCapacity() const { return MagazineCapacity; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetAmmo() const { return Ammo;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetMaxAmmo() const { return MaxAmmo; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	int32 GetTotalAmmo() const { return Ammo + AmmoInMagazine;}

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool IsReloading() { return GetWorldTimerManager().IsTimerActive(ReloadTimer); }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	float GetRemainingReloadTime() { return GetWorldTimerManager().GetTimerRemaining(ReloadTimer); }

	// ------ SETTERS ------

	UFUNCTION(BlueprintCallable)
	int32 AddAmmo(int32 AmmoToAdd) { return Ammo += AmmoToAdd; }

	UFUNCTION(BlueprintCallable)
	int32 RemoveAmmo(int32 AmmoToSubtract) { return Ammo -= AmmoToSubtract; }

	UFUNCTION(BlueprintCallable)
	int32 SetAmmo(int32 NewAmmo) { return Ammo = NewAmmo; }

	UFUNCTION(BlueprintCallable)
	int32 SetAmmoInMagazine(int32 NewAmmo) { return AmmoInMagazine = NewAmmo; }
};
