// Fill out your copyright notice in the Description page of Project Settings.


#include "Characters/PlayerCharacter.h"

#include "Kismet/KismetMathLibrary.h"

APlayerCharacter::APlayerCharacter()
{
	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	MeleeWeapon = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Melee Mesh"));
	MeleeWeapon->SetupAttachment(GetMesh());
	
	Camera = CreateDefaultSubobject<UCameraComponent>(TEXT("CameraComponent"));
	Camera->SetupAttachment(GetRootComponent());
	Camera->AddRelativeLocation(FVector(0, 0, 2400));
	Camera->SetRelativeRotation(FRotator(0.0f, -85.0f, 0.0f));
	Camera->bUsePawnControlRotation = false;
}

void APlayerCharacter::BeginPlay()
{
	Super::BeginPlay();

	Camera->SetWorldRotation(FRotator(-85.0f, 0.0f, 0.0f));
	// Camera->DetachFromComponent(FDetachmentTransformRules::KeepWorldTransform);
}

void APlayerCharacter::UpdateCameraLocation()
{
	const FVector CameraXYLocation = FVector(Camera->GetComponentLocation().X, Camera->GetComponentLocation().Y, 0.0f);
	const FVector PlayerXYLocation = FVector(GetMesh()->GetComponentLocation().X, GetMesh()->GetComponentLocation().Y, 0.0f);

	if (FVector::Distance(CameraXYLocation, PlayerXYLocation) > DistanceForCameraToFollow)
	{
		const FVector CamMovementDirection = UKismetMathLibrary::GetDirectionUnitVector(CameraXYLocation, PlayerXYLocation);
		const float DeltaTime = GetWorld()->DeltaTimeSeconds;

		Camera->SetWorldLocation(Camera->GetComponentLocation() + CamMovementDirection * CameraMoveSpeed * DeltaTime);
		
		// if (FVector::Distance(CameraXYLocation, PlayerXYLocation) < CameraMoveSpeed * DeltaTime)
		// {
		// 	Camera->SetWorldLocation(GetMesh()->GetComponentLocation() + FVector::UpVector * 2400.0f);
		// }
		// else
		// {
		// 	
		// }

		FVector ZHeight = FVector(Camera->GetComponentLocation().X, Camera->GetComponentLocation().Y, GetMesh()->GetComponentLocation().Z + 2400.0f);
		Camera->SetWorldLocation(ZHeight);
	}
}

void APlayerCharacter::LightAttack()
{
	ComboTracker.Add(EMeleeAction::LightAttack);
	if (FindCombo())
	{
		GetWorldTimerManager().SetTimer(ComboResetTimer, this, &APlayerCharacter::ResetCombo, ComboResetDelay, false);
	}
	else
	 {
		ResetCombo();
		ComboTracker.Add(EMeleeAction::LightAttack);
	 }
}

void APlayerCharacter::HeavyAttack()
{
	ComboTracker.Add(EMeleeAction::HeavyAttack);
	if (FindCombo())
	{
		GetWorldTimerManager().SetTimer(ComboResetTimer, this, &APlayerCharacter::ResetCombo, ComboResetDelay, false);
	}
	else
	{
		ResetCombo();
		ComboTracker.Add(EMeleeAction::HeavyAttack);
	}
}

bool APlayerCharacter::FindCombo()
{
	// For loop in a for loop is gross but hopefully temporary.
	for (int32 i = 0; i < MeleeCombos.Num(); i++)
	{
		TArray<TEnumAsByte<EMeleeAction>> TargetCombo;
		for (int32 k = 0; k < MeleeCombos[i].Attacks.Num(); k++)
		{
			TargetCombo.Add(MeleeCombos[i].Attacks[k].Action);
		}

		for (int32 z = 0; z < ComboTracker.Num(); z++)
		{
			TArray<EMeleeAction> TempTargetCombo;
			TempTargetCombo.Append(TargetCombo);
			TempTargetCombo.SetNum(ComboTracker.Num());
			
			if (ComboTracker != TempTargetCombo)
			{
				continue;
			}

			if (z == ComboTracker.Num() - 1)
			{
				CurrentActiveCombo = MeleeCombos[i];
				return true;
			}
		}
	}

	CurrentActiveCombo = FMeleeCombo();
	return false;
}

void APlayerCharacter::ResetCombo()
{
	ComboTracker.Empty();
}

void APlayerCharacter::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);

	UpdateCameraLocation();

	if (bShowDebugLogs == true)
	{

		// --- RANGED WEAPON DEBUG --- //
		
		// Debug message to show how much ammo the equipped weapon has.
		if (EquippedRangedWeapon) GEngine->AddOnScreenDebugMessage(900, 1.0f, FColor::Green,
		"Ammo: " + FString::FromInt(EquippedRangedWeapon->GetAmmoInMagazine()) + " / " +  FString::FromInt(EquippedRangedWeapon->GetAmmo()));
		
		// Debug message to show if the weapon is currently reloading.
		if (EquippedRangedWeapon && EquippedRangedWeapon->IsReloading())
		{
			GEngine->AddOnScreenDebugMessage(901, EquippedRangedWeapon->GetRemainingReloadTime(), FColor::Orange,
				"Reloading: " + FString::SanitizeFloat(EquippedRangedWeapon->GetRemainingReloadTime()));
		}
		
		// Debug message to show what weapons are being carried.
		FString WeaponNames = "";
		for (int32 i = 0; i < RangedWeapons.Num(); i++)
		{
			if (i == RangedWeapons.Num() - 1)
			{
				WeaponNames += RangedWeapons[i]->GetDisplayName();
			}
			else
			{
				WeaponNames += RangedWeapons[i]->GetDisplayName() + ", ";
			}
		}
		GEngine->AddOnScreenDebugMessage(902, 1.0f, FColor::White, "Carried RangedWeapons: " + WeaponNames);
		GEngine->AddOnScreenDebugMessage(999, 1.0f, FColor::Black, " "); // Empty space to separate ranged and melee debug.

		// --- MELEE DEBUG --- //

		// Debug message to show what combo inputs have been used so far.
		FString ComboInput;
		for (int32 i = 0; i < ComboTracker.Num(); i++)
		{
			switch (ComboTracker[i])
			{
			case EMeleeAction::LightAttack:
				ComboInput.Append("LIGHT");
				break;
			case EMeleeAction::HeavyAttack:
				ComboInput.Append("HEAVY");
				break;
			}
			if (i < ComboTracker.Num() - 1) ComboInput.Append(", ");
		}
		GEngine->AddOnScreenDebugMessage(800, 1.0f, FColor::Yellow, "Combo Input: " + ComboInput);

		// TODO: Debug message to show what combo is currently being used.
		if (CurrentActiveCombo != FMeleeCombo())
		{
			FString ComboName = "Current Combo Name: ";
			ComboName.Append(CurrentActiveCombo.ComboName);
			GEngine->AddOnScreenDebugMessage(801, 1.0f, FColor::Orange, ComboName);
		}
	}
}
