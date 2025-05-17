// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetCollisionProfileName("Weapon");
	SkeletalMesh->SetGenerateOverlapEvents(true);
	SkeletalMesh->CanCharacterStepUpOn = ECB_No;

	Arrow = CreateDefaultSubobject<UArrowComponent>(TEXT("Arrow"));
	Arrow->SetupAttachment(SkeletalMesh);
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();

	AmmoInMagazine = MagazineCapacity;
}

void AWeapon::Fire_Implementation()
{
	if (!isAutomatic && isTriggerPulled) return;
	if (isAutomatic && GetWorldTimerManager().IsTimerActive(FireRateTimer)) return;
	if (AmmoInMagazine <= 0) return;
	if (IsReloading()) return;
	
	ShootProjectile();

	isTriggerPulled = true;

	GetWorldTimerManager().SetTimer(FireRateTimer, RPMToSeconds(FireRateRpm), false);
}

void AWeapon::ReleaseFire()
{
	isTriggerPulled = false;

	GetWorldTimerManager().ClearTimer(FireRateTimer);
}

void AWeapon::ShootProjectile()
{
	if (Projectile == nullptr) return;

	if (ProjectileCount < 1) ProjectileCount = 1;
	for (int32 i = 0; i < ProjectileCount; i++)
	{
		AProjectile* SpawnedProjectile = GetWorld()->SpawnActor<AProjectile>(Projectile);
    
        SpawnedProjectile->SetActorLocation(GetArrowComponent()->GetComponentLocation());
        SpawnedProjectile->GetProjectileMovement()->Velocity = Arrow->GetForwardVector().RotateAngleAxis(FMath::RandRange(-ProjectileSpread, ProjectileSpread), GetActorUpVector()) * ProjectileSpeed;
        SpawnedProjectile->SetDamage(Damage);
	}
	
	
	AmmoInMagazine -= 1;
}

void AWeapon::Reload()
{
	if (IsReloading()) return;
	
	if (Ammo == 0) return;
	
	GetWorldTimerManager().SetTimer(ReloadTimer, this, &AWeapon::FinishReload, ReloadTime, false);
}

void AWeapon::CancelReload()
{
	if (!IsReloading()) return;
	
	GetWorldTimerManager().ClearTimer(ReloadTimer);
}

void AWeapon::FinishReload()
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

void AWeapon::SetWeaponCollision(bool Input)
{
	if (Input == true)
	{
		SkeletalMesh->SetCollisionProfileName("Weapon");
		SkeletalMesh->SetSimulatePhysics(true);
		SkeletalMesh->SetGenerateOverlapEvents(true);
	}
	else
	{
		SkeletalMesh->SetSimulatePhysics(false);
		SkeletalMesh->SetCollisionProfileName("NoCollision");
		SkeletalMesh->SetGenerateOverlapEvents(false);
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	if (CanBePickedUp() == false && IsHidden() == false)
	{
		DrawDebugLine(GetWorld(), Arrow->GetComponentLocation(), Arrow->GetComponentLocation() + Arrow->GetForwardVector() * 100.0f, FColor::Red);
	}
}

