// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
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

	// ------ WEAPON VARIABLES ------

	UPROPERTY(EditAnywhere, Category="Weapon")
	FString DisplayName = "Unnamed Weapon";
	
	// The Projectile that this weapon fires.
	UPROPERTY(EditAnywhere, Category="Weapon")
	TSubclassOf<AProjectile> Projectile;

	UPROPERTY(EditAnywhere, Category="Weapon")
	float Damage = 10.0f;

	UPROPERTY(EditAnywhere, Category="Weapon")
	float ProjectileSpeed = 2000.0f;

	UPROPERTY(EditAnywhere, Category="Weapon")
	float ProjectileSpread = 2.0f;

	// How many projectiles are spawned when the weapon is fired.
	UPROPERTY(EditAnywhere, Category="Weapon", meta=(UIMin=1, ClampMin=1))
	int32 ProjectileCount = 1;

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

	UFUNCTION(BlueprintCallable)
	void Reload();

	UFUNCTION(BlueprintCallable)
	void CancelReload();

	UFUNCTION()
	void FinishReload();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(bool Input);
	
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
	FString GetDisplayName() { return DisplayName; }

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

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanBePickedUp() { return !GetAttachParentActor(); }

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
