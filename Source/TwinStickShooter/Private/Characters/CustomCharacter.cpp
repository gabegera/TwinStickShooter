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

void ACustomCharacter::UpdateRotation()
{
	if (TargetRotation != FRotator::ZeroRotator)
	{
		float DeltaTime = GetWorld()->GetDeltaSeconds();
		float CurrentYaw = GetMesh()->GetRelativeRotation().Clamp().Yaw;
		float TargetYaw = TargetRotation.Clamp().Yaw;
		
		if (CurrentYaw < TargetYaw)
		{
			if (FMath::Abs(CurrentYaw - TargetYaw) < 180)
			{
				GetMesh()->SetRelativeRotation(GetMesh()->GetRelativeRotation() + FRotator(0, MaxRotationSpeed * DeltaTime, 0));
			}
			else GetMesh()->SetRelativeRotation(GetMesh()->GetRelativeRotation() + FRotator(0, -MaxRotationSpeed * DeltaTime, 0));
		}
		else
		{
			if (FMath::Abs(CurrentYaw - TargetYaw) < 180)
			{
				GetMesh()->SetRelativeRotation(GetMesh()->GetRelativeRotation() + FRotator(0, -MaxRotationSpeed * DeltaTime, 0));
			}
			else GetMesh()->SetRelativeRotation(GetMesh()->GetRelativeRotation() + FRotator(0, MaxRotationSpeed * DeltaTime, 0));
		}

		if (FMath::Abs(CurrentYaw - TargetYaw) < MaxRotationSpeed * DeltaTime)
		{
			GetMesh()->SetRelativeRotation(TargetRotation);
		}
	}
}

// Called when the game starts or when spawned
void ACustomCharacter::BeginPlay()
{
	Super::BeginPlay();

	CurrentHealth = MaxHealth;

	// Adds the starting weapons to the player inventory.
	TArray StartingWeaponsArray = StartingRangedWeapons.Array();
	RangedWeapons.Empty();
	for (int32 i = 0; i < StartingWeaponsArray.Num(); i++)
	{
		RangedWeapons.Add(SpawnRangedWeapon(StartingWeaponsArray[i]));
	}

	if (EquippedRangedWeapon == nullptr && RangedWeapons.IsEmpty() == false)
	{
		EquipWeapon(RangedWeapons[0]);
	}
}

void ACustomCharacter::OnConstruction(const FTransform& Transform)
{
	Super::OnConstruction(Transform);

	// while (StartingRangedWeapons.Num() > MaxWeaponCapacity) StartingRangedWeapons.Remove(StartingRangedWeapons.Array().Last());
}

void ACustomCharacter::OnOverlapBegin(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	if (!OtherActor) return;

	if (NearestInteractable == nullptr) NearestInteractable = OtherActor;
	else if (GetDistanceTo(NearestInteractable) > GetDistanceTo(OtherActor)) NearestInteractable = OtherActor;

	
	if (OtherActor->FindNearestCommonBaseClass(ARangedWeapon::StaticClass()) == ARangedWeapon::StaticClass())
	{
		ARangedWeapon* OverlappedWeapon = Cast<ARangedWeapon>(OtherActor);

		// If carrying a weapon of the same type on the ground, pick up its ammo.
		for (int32 i = 0; i < RangedWeapons.Num(); i++)
		{
			if (RangedWeapons[i]->GetClass() != OverlappedWeapon->GetClass()) continue;

			// If the additional ammo won't go over the limit, pick it all up.
			if (RangedWeapons[i]->GetAmmo() + OverlappedWeapon->GetAmmo() <= RangedWeapons[i]->GetMaxAmmo())
			{
				RangedWeapons[i]->AddAmmo(OverlappedWeapon->GetAmmo());
				OverlappedWeapon->SetAmmo(0);
				return;
			}
			else // If the ammo goes over the maximum limit, pickup what fits.
			{
				int32 AmmoSpaceAvailable = RangedWeapons[i]->GetMaxAmmo() - RangedWeapons[i]->GetAmmo();
				RangedWeapons[i]->AddAmmo(AmmoSpaceAvailable);
				OverlappedWeapon->RemoveAmmo(AmmoSpaceAvailable);
				return;
			}
		}
		
		if (RangedWeapons.Num() < MaxWeaponCapacity && OverlappedWeapon->CanBePickedUp()) PickupWeapon(OverlappedWeapon);
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
	if (EquippedRangedWeapon == nullptr) return;

	EquippedRangedWeapon->SetInstigator(this);
	EquippedRangedWeapon->Fire();
}

void ACustomCharacter::StopUsingWeapon()
{
	if (EquippedRangedWeapon == nullptr) return;

	EquippedRangedWeapon->ReleaseFire();
}

void ACustomCharacter::ReloadWeapon()
{
	if (EquippedRangedWeapon == nullptr) return;
	
	EquippedRangedWeapon->Reload();
}

ARangedWeapon* ACustomCharacter::SpawnRangedWeapon(TSubclassOf<ARangedWeapon> WeaponToSpawn)
{
	ARangedWeapon* SpawnedWeapon = GetWorld()->SpawnActor<ARangedWeapon>(WeaponToSpawn);
	
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
	if (EquippedRangedWeapon == nullptr)
	{
		EquipWeapon(RangedWeapons[0]);

		return;
	}

	// Finds where the next weapon in the array is located.
	int32 NextWeaponIndex = 0;
	for (int32 i = 0; i < RangedWeapons.Num(); i++)
	{
		if (RangedWeapons[i]->GetClass() == EquippedRangedWeapon->GetClass())
		{
			NextWeaponIndex = i + 1;
			break;
		}
	}
	
	// Equips the next weapon in the array. If the next weapon is outside the bounds of the array then equips the first weapon.
	if (NextWeaponIndex >= RangedWeapons.Num())
	{
		EquipWeapon(RangedWeapons[0]);
	}
	else EquipWeapon(RangedWeapons[NextWeaponIndex]);
}

void ACustomCharacter::EquipWeapon(ARangedWeapon* NewWeapon)
{
	if (RangedWeapons.IsEmpty()) return;

	// Hides all non-equipped weapons and cancels any reloads.
	for (int i = 0; i < RangedWeapons.Num(); i++)
	{
		RangedWeapons[i]->CancelReload();
		
		if (RangedWeapons[i]->GetClass() != NewWeapon->GetClass())
		{
			RangedWeapons[i]->SetActorHiddenInGame(true);
			RangedWeapons[i]->SetWeaponCollision(false);
		}
	}
	
	EquippedRangedWeapon = NewWeapon;
	EquippedRangedWeapon->SetActorHiddenInGame(false);
	EquippedRangedWeapon->SetWeaponCollision(false);
}

void ACustomCharacter::PickupWeapon(ARangedWeapon* NewWeapon)
{
	NewWeapon->SetActorHiddenInGame(true);
	NewWeapon->SetWeaponCollision(false);
	NewWeapon->GetSkeletalMesh()->SetRelativeLocation(FVector(0, 0, 0));
	NewWeapon->GetSkeletalMesh()->SetRelativeRotation(FRotator(0, 0, 0));
	NewWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	NewWeapon->SetActorRotation(GetMesh()->GetComponentRotation());
	NewWeapon->SetActorRelativeLocation(FVector(0, 40, 100));
	
	// If there is room in the weapons array, add the weapon and return.
	if (RangedWeapons.Num() < MaxWeaponCapacity)
	{
		RangedWeapons.Add(NewWeapon);
		if (EquippedRangedWeapon == nullptr) EquipWeapon(NewWeapon);

		return;
	}

	// Drops the currently equipped weapon.
	if (EquippedRangedWeapon)
	{
		EquippedRangedWeapon->DetachFromActor(FDetachmentTransformRules::KeepWorldTransform);
		EquippedRangedWeapon->SetActorLocation(GetMesh()->GetComponentLocation() + GetMesh()->GetForwardVector() * 20.0f + FVector(0, 0, 20));
		EquippedRangedWeapon->SetActorHiddenInGame(false);
		EquippedRangedWeapon->SetWeaponCollision(true);
	}

	// Replaces equipped weapon in the array with a new weapon. 
	for (int32 i = 0; i < RangedWeapons.Num(); i++)
	{
		if (RangedWeapons[i]->GetClass() == EquippedRangedWeapon->GetClass())
		{
			RangedWeapons[i] = NewWeapon;
			EquipWeapon(NewWeapon);
		}
	}
}

void ACustomCharacter::Interact()
{
	if (NearestInteractable == nullptr) return;
	
	if (NearestInteractable->FindNearestCommonBaseClass(ARangedWeapon::StaticClass()) == ARangedWeapon::StaticClass())
	{
		ARangedWeapon* NearestWeapon = Cast<ARangedWeapon>(NearestInteractable);
		if (NearestWeapon->CanBePickedUp()) PickupWeapon(NearestWeapon);
	}
}

// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	
	UpdateRotation();
}

