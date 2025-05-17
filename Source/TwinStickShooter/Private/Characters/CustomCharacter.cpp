// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CustomCharacter.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/PawnMovementComponent.h"

// Sets default values
ACustomCharacter::ACustomCharacter()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	SphereCollider = CreateDefaultSubobject<USphereComponent>(TEXT("SphereCollider"));
	SphereCollider->SetupAttachment(GetMesh());
	SphereCollider->SetGenerateOverlapEvents(true);
	SphereCollider->OnComponentBeginOverlap.AddDynamic(this, &ACustomCharacter::OnOverlapBegin);
	SphereCollider->OnComponentEndOverlap.AddDynamic(this, &ACustomCharacter::OnOverlapEnd);
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

	if (EquippedWeapon == nullptr && Weapons.IsEmpty() == false)
	{
		EquipWeapon(Weapons[0]);
	}
}

void ACustomCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// while (StartingWeapons.Num() > MaxWeaponCapacity) StartingWeapons.Remove(StartingWeapons.Array().Last());
}

void ACustomCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	if (NearestInteractable == nullptr) NearestInteractable = OtherActor;
	else if (GetDistanceTo(NearestInteractable) > GetDistanceTo(OtherActor)) NearestInteractable = OtherActor;

	
	
	if (OtherActor->FindNearestCommonBaseClass(AWeapon::StaticClass()) == AWeapon::StaticClass())
	{
		AWeapon* OverlappedWeapon = Cast<AWeapon>(OtherActor);

		// If carrying a weapon of the same type on the ground, pick up its ammo.
		for (int32 i = 0; i < Weapons.Num(); i++)
		{
			if (Weapons[i]->GetClass() != OverlappedWeapon->GetClass()) continue;

			// If the additional ammo won't go over the limit, pick it all up.
			if (Weapons[i]->GetAmmo() + OverlappedWeapon->GetAmmo() <= Weapons[i]->GetMaxAmmo())
			{
				Weapons[i]->AddAmmo(OverlappedWeapon->GetAmmo());
				OverlappedWeapon->SetAmmo(0);
				return;
			}
			else // If the ammo goes over the maximum limit, pickup what fits.
			{
				int32 AmmoSpaceAvailable = Weapons[i]->GetMaxAmmo() - Weapons[i]->GetAmmo();
				Weapons[i]->AddAmmo(AmmoSpaceAvailable);
				OverlappedWeapon->RemoveAmmo(AmmoSpaceAvailable);
				return;
			}
		}
		
		if (Weapons.Num() < MaxWeaponCapacity && OverlappedWeapon->CanBePickedUp()) PickupWeapon(OverlappedWeapon);
	}
}

void ACustomCharacter::OnOverlapEnd(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex)
{
	if (!OtherActor) return;
	
	if (OtherActor == NearestInteractable) NearestInteractable = nullptr;
}

void ACustomCharacter::Dash(FVector DashDirection)
{
	if (GetMovementComponent()->GetVelocityForNavMovement() == FVector::ZeroVector) return;
	if (GetWorldTimerManager().IsTimerActive(DashTimer)) return;
	if (GetWorldTimerManager().IsTimerActive(DashCooldownTimer)) return;

	DashDirection.Normalize();
	UpdateDash(DashDirection * DashSpeed);
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
	
	SpawnedWeapon->SetActorHiddenInGame(true);
	SpawnedWeapon->SetWeaponCollision(false);
	SpawnedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	SpawnedWeapon->SetActorRotation(GetMesh()->GetComponentRotation());
	SpawnedWeapon->SetActorRelativeLocation(FVector(0, 40, 100));
	
	return SpawnedWeapon;
}

void ACustomCharacter::EquipNextWeapon()
{
	// If there is no weapon equipped then equip the first weapon available.
	if (EquippedWeapon == nullptr)
	{
		EquipWeapon(Weapons[0]);

		return;
	}

	// Finds where the next weapon in the array is located.
	int32 NextWeaponIndex = 0;
	for (int32 i = 0; i < Weapons.Num(); i++)
	{
		if (Weapons[i]->GetClass() == EquippedWeapon->GetClass())
		{
			NextWeaponIndex = i + 1;
			break;
		}
	}
	
	// Equips the next weapon in the array. If the next weapon is outside the bounds of the array then equips the first weapon.
	if (NextWeaponIndex >= Weapons.Num())
	{
		EquipWeapon(Weapons[0]);
	}
	else EquipWeapon(Weapons[NextWeaponIndex]);
}

void ACustomCharacter::EquipWeapon(AWeapon* NewWeapon)
{
	if (Weapons.IsEmpty()) return;

	// Hides all non-equipped weapons and cancels any reloads.
	for (int i = 0; i < Weapons.Num(); i++)
	{
		Weapons[i]->CancelReload();
		
		if (Weapons[i]->GetClass() != NewWeapon->GetClass())
		{
			Weapons[i]->SetActorHiddenInGame(true);
			Weapons[i]->SetWeaponCollision(false);
		}
	}
	
	EquippedWeapon = NewWeapon;
	EquippedWeapon->SetActorHiddenInGame(false);
	EquippedWeapon->SetWeaponCollision(false);
}

void ACustomCharacter::PickupWeapon(AWeapon* NewWeapon)
{
	NewWeapon->SetActorHiddenInGame(true);
	NewWeapon->SetWeaponCollision(false);
	NewWeapon->GetSkeletalMesh()->SetRelativeLocation(FVector(0, 0, 0));
	NewWeapon->GetSkeletalMesh()->SetRelativeRotation(FRotator(0, 0, 0));
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	NewWeapon->SetActorRotation(GetMesh()->GetComponentRotation());
	NewWeapon->SetActorRelativeLocation(FVector(0, 40, 100));
	
	// If there is room in the weapons array, add the weapon and return.
	if (Weapons.Num() < MaxWeaponCapacity)
	{
		Weapons.Add(NewWeapon);
		if (EquippedWeapon == nullptr) EquipWeapon(NewWeapon);

		return;
	}

	// Drops the currently equipped weapon.
	if (EquippedWeapon)
	{
		EquippedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedWeapon->SetActorLocation(GetMesh()->GetComponentLocation() + GetMesh()->GetForwardVector() * 20.0f + FVector(0, 0, 20));
		EquippedWeapon->SetActorHiddenInGame(false);
		EquippedWeapon->SetWeaponCollision(true);
	}

	// Replaces equipped weapon in the array with a new weapon. 
	for (int32 i = 0; i < Weapons.Num(); i++)
	{
		if (Weapons[i]->GetClass() == EquippedWeapon->GetClass())
		{
			Weapons[i] = NewWeapon;
			EquipWeapon(NewWeapon);
		}
	}
}

void ACustomCharacter::Interact()
{
	if (NearestInteractable == nullptr) return;
	
	if (NearestInteractable->FindNearestCommonBaseClass(AWeapon::StaticClass()) == AWeapon::StaticClass())
	{
		AWeapon* NearestWeapon = Cast<AWeapon>(NearestInteractable);
		if (NearestWeapon->CanBePickedUp()) PickupWeapon(NearestWeapon);
	}
}

// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

