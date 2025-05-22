// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/RangedWeapon.h"


// Sets default values
ARangedWeapon::ARangedWeapon()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(SkeletalMesh);
}

// Called when the game starts or when spawned
void ARangedWeapon::BeginPlay()
{
	Super::BeginPlay();

	AmmoInMagazine = MagazineCapacity;
}

void ARangedWeapon::Fire()
{
	if (!isAutomatic && isTriggerPulled) return;
	if (isAutomatic && GetWorldTimerManager().IsTimerActive(FireRateTimer)) return;
	if (GetWorldTimerManager().IsTimerActive(TriggerDelayTimer)) return;
	if (AmmoInMagazine <= 0) return;
	if (IsReloading()) return;
	
	ShootProjectile();

	isTriggerPulled = true;

	GetWorldTimerManager().SetTimer(FireRateTimer, RPMToSeconds(FireRateRpm), false);
}

void ARangedWeapon::ReleaseFire()
{
	isTriggerPulled = false;

	GetWorldTimerManager().ClearTimer(FireRateTimer);
	GetWorldTimerManager().SetTimer(TriggerDelayTimer, TriggerDelay, false);
}

void ARangedWeapon::ShootProjectile()
{
	if (Projectile == nullptr) return;

	if (ProjectileCount < 1) ProjectileCount = 1;
	for (int32 i = 0; i < ProjectileCount; i++)
	{
		AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile);

		SpawnedProjectile->SetInstigator(GetInstigator());
		SpawnedProjectile->SetSpawner(this);
		SpawnedProjectile->SetActorLocation(GetArrowComponent()->GetComponentLocation());
		SpawnedProjectile->GetProjectileMovement()->Velocity = Arrow->GetForwardVector().RotateAngleAxis(FMath::RandRange(-ProjectileSpread, ProjectileSpread), GetActorUpVector()) * ProjectileSpeed;
		SpawnedProjectile->SetDamage(Damage);
	}
	
	
	AmmoInMagazine -= 1;
}

void ARangedWeapon::Reload()
{
	if (IsReloading()) return;
	
	if (Ammo == 0) return;
	
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &ARangedWeapon::FinishReload, ReloadTime, false);
}

void ARangedWeapon::CancelReload()
{
	if (!IsReloading()) return;
	
	GetWorldTimerManager().ClearTimer(ReloadTimer);
}

void ARangedWeapon::FinishReload()
{
	if (Ammo - (MagazineCapacity - AmmoInMagazine) < 0)
	{
		AmmoInMagazine = Ammo;
		Ammo = 0;
	}
	else
	{
		Ammo -= MagazineCapacity - AmmoInMagazine;
		AmmoInMagazine = MagazineCapacity;
	}
}

// Called every frame
void ARangedWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanBePickedUp() == false && IsHidden() == false)
	{
		DrawDebugLine(GetWorld(), Arrow->GetComponentLocation(), Arrow->GetComponentLocation() + Arrow->GetForwardVector() * 100.0f, FColor::Red);
	}
}

