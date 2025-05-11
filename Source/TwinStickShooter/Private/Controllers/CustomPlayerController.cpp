// Fill out your copyright notice in the Description page of Project Settings.


#include "Controllers/CustomPlayerController.h"

#include "Components/CapsuleComponent.h"
#include "GameFramework/Character.h"

void ACustomPlayerController::SetupPlayerInputComponent(UInputComponent* PlayerInputComponent)
{
	UEnhancedInputComponent* Input = Cast<UEnhancedInputComponent>(PlayerInputComponent);
	// You can bind to any of the trigger events here by changing the "ETriggerEvent" enum value
	Input->BindAction(Input_Move, ETriggerEvent::Triggered, this, &ACustomPlayerController::Move);
	Input->BindAction(Input_Dodge, ETriggerEvent::Triggered, this, &ACustomPlayerController::Dodge);
	Input->BindAction(Input_Parry, ETriggerEvent::Triggered, this, &ACustomPlayerController::Parry);
	
	Input->BindAction(Input_Aim, ETriggerEvent::Triggered, this, &ACustomPlayerController::Aim);
	Input->BindAction(Input_Aim, ETriggerEvent::Completed, this, &ACustomPlayerController::Aim);

	Input->BindAction(Input_Shoot, ETriggerEvent::Triggered, this, &ACustomPlayerController::Shoot);
	Input->BindAction(Input_Shoot, ETriggerEvent::Completed, this, &ACustomPlayerController::ReleaseShoot);
	
	Input->BindAction(Input_Reload, ETriggerEvent::Triggered, this, &ACustomPlayerController::ReloadWeapon);

	Input->BindAction(Input_NextWeapon, ETriggerEvent::Triggered, this, &ACustomPlayerController::EquipNextWeapon);
}

void ACustomPlayerController::BeginPlay()
{
	Super::BeginPlay();

	if (ULocalPlayer* LocalPlayer = Cast<ULocalPlayer>(Player))
	{
		if (UEnhancedInputLocalPlayerSubsystem* InputSystem = LocalPlayer->GetSubsystem<UEnhancedInputLocalPlayerSubsystem>())
		{
			if (!InputMapping.IsNull())
			{
				InputSystem->AddMappingContext(InputMapping.LoadSynchronous(), 0);
			}
		}

		SetupPlayerInputComponent(InputComponent);
	}
}

void ACustomPlayerController::Move(const FInputActionInstance& Instance)
{
	if (GetWorldTimerManager().IsTimerActive(GetCustomCharacter()->GetDashTimer())) return;
	
	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	
	GetCharacter()->AddMovementInput(FVector::RightVector, Value.X);
	GetCharacter()->AddMovementInput(FVector::ForwardVector, Value.Y);

	if (!isAiming)
	{
		GetCharacter()->GetMesh()->SetWorldRotation(FVector(Value.X, -Value.Y, 0).Rotation());
	}
}

void ACustomPlayerController::Dodge()
{
	GetCustomCharacter()->Dash();
}

void ACustomPlayerController::Parry()
{
	
}

void ACustomPlayerController::Aim(const FInputActionInstance& Instance)
{
	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	
	if (Value.IsZero())
	{
		isAiming = false;
		return;
	}
	
	GetCharacter()->GetMesh()->SetWorldRotation(FVector(Value.X, Value.Y, 0).Rotation());
	isAiming = true;
}

void ACustomPlayerController::Shoot()
{
	if (GetCustomCharacter()->GetEquippedWeapon()->GetAmmoInMagazine() <= 0 && ShouldReloadOnFire)
	{
		GetCustomCharacter()->ReloadWeapon();
	}
	
	GetCustomCharacter()->UseWeapon();
}

void ACustomPlayerController::ReleaseShoot()
{
	GetCustomCharacter()->StopUsingWeapon();
}

void ACustomPlayerController::ReloadWeapon()
{
	GetCustomCharacter()->ReloadWeapon();
}

void ACustomPlayerController::EquipNextWeapon()
{
	GetCustomCharacter()->EquipNextWeapon();
}

// ------ GETTERS ------

ACustomCharacter* ACustomPlayerController::GetCustomCharacter()
{
	if (PossessedCustomCharacter == nullptr)
	{
		PossessedCustomCharacter = Cast<ACustomCharacter>(GetCharacter());
	}

	return PossessedCustomCharacter;
}
