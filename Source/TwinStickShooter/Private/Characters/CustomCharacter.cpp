// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CustomCharacter.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ACustomCharacter::ACustomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
}

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	// Adds the starting weapons to the player inventory.
	TArray StartingWeaponsArray = StartingWeapons.Array();
	Weapons.Empty();
	for (int32 i = 0; i < StartingWeaponsArray.Num(); i++)
	{
		Weapons.Add(SpawnWeapon(StartingWeaponsArray[i]));
	}

	if (EquippedWeapon == nullptr)
	{
		EquipWeapon(Weapons[0]);
	}
}

void ACustomCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// while (StartingWeapons.Num() > MaxWeaponCapacity) StartingWeapons.Remove(StartingWeapons.Array().Last());
}

void ACustomCharacter::Dash()
{
	if (GetMovementComponent()->GetVelocityForNavMovement() == FVector::ZeroVector) return;
	if (GetWorldTimerManager().IsTimerActive(DashTimer)) return;
	if (GetWorldTimerManager().IsTimerActive(DashCooldownTimer)) return;
	
	FVector Direction = FVector::ZeroVector;
	float Speed = 0;
	
	GetMovementComponent()->GetVelocityForNavMovement().ToDirectionAndLength(Direction, Speed);
	
	UpdateDash(Direction * DashSpeed);
	GetWorldTimerManager().SetTimer(DashTimer, this, &ACustomCharacter::StopDash, DashTime, false);
}

void ACustomCharacter::UpdateDash(FVector DashDirectionAndSpeed)
{
	GetMovementComponent()->Velocity = DashDirectionAndSpeed;
	
	FTimerDelegate DashDelegate;
	DashDelegate.BindUFunction(this, FName("UpdateDash"), DashDirectionAndSpeed);
	GetWorldTimerManager().SetTimer(DashUpdateTimer, DashDelegate, 0.00001f, false);
}

void ACustomCharacter::StopDash()
{
	if (GetWorldTimerManager().IsTimerActive(DashUpdateTimer))
	{
		GetWorldTimerManager().ClearTimer(DashUpdateTimer);
	}
	
	GetMovementComponent()->Velocity = FVector::ZeroVector;

	GetWorldTimerManager().SetTimer(DashCooldownTimer, DashCooldown, false);
}

void ACustomCharacter::UseWeapon()
{
	if (EquippedWeapon == nullptr) return;

	EquippedWeapon->Fire();
}

void ACustomCharacter::StopUsingWeapon()
{
	if (EquippedWeapon == nullptr) return;

	EquippedWeapon->ReleaseFire();
}

void ACustomCharacter::ReloadWeapon()
{
	if (EquippedWeapon == nullptr) return;
	
	EquippedWeapon->Reload();
}

AWeapon* ACustomCharacter::SpawnWeapon(TSubclassOf<AWeapon> WeaponToSpawn)
{
	AWeapon* SpawnedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponToSpawn);

	SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	SpawnedWeapon->SetActorRotation(GetMesh()->GetComponentRotation());
	SpawnedWeapon->SetActorRelativeLocation(FVector(0, 40, 100));

	return SpawnedWeapon;
}

void ACustomCharacter::EquipNextWeapon()
{
	if (EquippedWeapon == nullptr)
	{
		EquipWeapon(Weapons[0]);

		return;
	}

	int32 NextWeaponIndex = 0;
	for (int32 i = 0; i < Weapons.Num(); i++)
	{
		if (Weapons[i]->GetClass() == EquippedWeapon->GetClass())
		{
			NextWeaponIndex = i + 1;
			break;
		}
	}

	if (NextWeaponIndex >= Weapons.Num())
	{
		EquipWeapon(Weapons[0]);
	}
	else EquipWeapon(Weapons[NextWeaponIndex]);
}

void ACustomCharacter::EquipWeapon(AWeapon* NewWeapon)
{
	if (Weapons.IsEmpty()) return;

	// for (int32 i = 0; i < Weapons.Num(); i++)
	// {
	// 	if (EquippedWeapon == nullptr) break;
	// 	
	// 	if (Weapons[i]->GetClass() == EquippedWeapon->GetClass())
	// 	{
	// 		
	// 		break;
	// 	}
	// }
	
	// if (EquippedWeapon) EquippedWeapon->Destroy();
	if (EquippedWeapon)
	{
		EquippedWeapon->SetActorHiddenInGame(true);
		EquippedWeapon->CancelReload();
	}
	
	EquippedWeapon = NewWeapon;
	EquippedWeapon->SetActorHiddenInGame(false);
}

// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

