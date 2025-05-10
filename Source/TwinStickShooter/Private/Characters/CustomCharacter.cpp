// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/CustomCharacter.h"
#include "Components/ArrowComponent.h"

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

	if (EquippedWeapon == nullptr)
	{
		EquipWeapon(0);
	}
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

void ACustomCharacter::EquipNextWeapon()
{
	if (EquippedWeapon == nullptr)
	{
		EquipWeapon(0);

		return;
	}

	int32 NextWeaponIndex = 0;
	for (int32 i = 0; i < Weapons.Num(); i++)
	{
		if (Weapons[i].GetDefaultObject() == EquippedWeapon->GetClass()->GetDefaultObject())
		{
			NextWeaponIndex = i + 1;
			break;
		}
	}

	if (NextWeaponIndex > Weapons.Num() - 1)
	{
		EquipWeapon(0);
	}
	else EquipWeapon(NextWeaponIndex);
}

void ACustomCharacter::EquipWeapon(int32 Index)
{
	if (Weapons.IsEmpty()) return;

	if (Index > Weapons.Num() - 1 || Index < 0) Index = 0;

	if (EquippedWeapon) EquippedWeapon->Destroy();
	EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(Weapons[Index]);

	if (EquippedWeapon == nullptr) return;
	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	EquippedWeapon->SetActorRotation(GetMesh()->GetComponentRotation());
	EquippedWeapon->SetActorRelativeLocation(FVector(0, 40, 100));
}

void ACustomCharacter::EquipWeapon(TSubclassOf<AWeapon> WeaponClass)
{
	if (Weapons.IsEmpty()) return;

	if (EquippedWeapon) EquippedWeapon->Destroy();
	EquippedWeapon = GetWorld()->SpawnActor<AWeapon>(WeaponClass);

	EquippedWeapon->AttachToComponent(GetMesh(), FAttachmentTransformRules::SnapToTargetIncludingScale);
	EquippedWeapon->SetActorRotation(GetMesh()->GetComponentRotation());
	EquippedWeapon->SetActorRelativeLocation(FVector(0, 40, 100));
}

// Called every frame
void ACustomCharacter::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

