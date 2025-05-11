// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

#include "Components/CapsuleComponent.h"

APlayerCharacter::APlayerCharacter()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));
	
	// Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	// Camera->SetupAttachment(GetCapsuleComponent());
	// Camera->AddRelativeLocation(FVector(200, 0, 1200));
	// Camera->bUsePawnControlRotation = false;
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	if (bShowDebugLogs == true)
	{
		// Debug message to show how much ammo the equipped weapon has.
		GEngine->AddOnScreenDebugMessage(900, 1.0f, FColor::Green,
		"Ammo: " + FString::FromInt(EquippedWeapon->GetAmmoInMagazine()) + " / " +  FString::FromInt(EquippedWeapon->GetAmmo()));
		
		// Debug message to show if the weapon is currently reloading.
		if (EquippedWeapon->IsReloading())
		{
			GEngine->AddOnScreenDebugMessage(901, EquippedWeapon->GetRemainingReloadTime(), FColor::Orange,
				"Reloading: " + FString::SanitizeFloat(EquippedWeapon->GetRemainingReloadTime()));
		}
		
		// Debug message to show what weapons are being carried.
		FString WeaponNames = "";
		for (int32 i = 0; i < Weapons.Num(); i++)
		{
			if (i == Weapons.Num() - 1)
			{
				WeaponNames += Weapons[i]->GetDisplayName();
			}
			else
			{
				WeaponNames += Weapons[i]->GetDisplayName() + ", ";
			}
		}
		GEngine->AddOnScreenDebugMessage(902, 1.0f, FColor::White, "Carried Weapons: " + WeaponNames);
			
	}
}
