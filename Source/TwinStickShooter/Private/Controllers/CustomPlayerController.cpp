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

	Input->BindAction(Input_LightAttack, ETriggerEvent::Triggered, this, &ACustomPlayerController::LightAttack);
	Input->BindAction(Input_HeavyAttack, ETriggerEvent::Triggered, this, &ACustomPlayerController::HeavyAttack);
	Input->BindAction(Input_Parry, ETriggerEvent::Triggered, this, &ACustomPlayerController::Parry);
	
	Input->BindAction(Input_Aim, ETriggerEvent::Triggered, this, &ACustomPlayerController::Aim);
	Input->BindAction(Input_Aim, ETriggerEvent::Completed, this, &ACustomPlayerController::Aim);

	Input->BindAction(Input_Shoot, ETriggerEvent::Triggered, this, &ACustomPlayerController::Shoot);
	Input->BindAction(Input_Shoot, ETriggerEvent::Completed, this, &ACustomPlayerController::ReleaseShoot);
	
	Input->BindAction(Input_Reload, ETriggerEvent::Triggered, this, &ACustomPlayerController::ReloadWeapon);

	Input->BindAction(Input_NextWeapon, ETriggerEvent::Triggered, this, &ACustomPlayerController::EquipNextWeapon);

	Input->BindAction(Input_Interact, ETriggerEvent::Triggered, this, &ACustomPlayerController::Interact);

	
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
	if (GetWorldTimerManager().IsTimerActive(GetPlayerCharacter()->GetDashTimer())) return;
	
	FVector2D Value = Instance.GetValue().Get<FVector2D>();
	FVector(Value.X, Value.Y, 0).ToDirectionAndLength(MovementDirection, MovementInputPercentage);

	if (!isAiming)
    {
    	GetPlayerCharacter()->SetTargetRotation(FVector(Value.X, Value.Y, 0).Rotation());
		GetPlayerCharacter()->AddMovementInput(GetPlayerCharacter()->GetMesh()->GetRightVector(), MovementInputPercentage);
    }
	else
	{
		GetCharacter()->AddMovementInput(FVector::RightVector, Value.X);
        GetCharacter()->AddMovementInput(FVector::ForwardVector, -Value.Y);
	}
}

void ACustomPlayerController::Dodge()
{
	GetPlayerCharacter()->Dash(MovementDirection);
}

void ACustomPlayerController::LightAttack()
{
	GetPlayerCharacter()->LightAttack();
}

void ACustomPlayerController::HeavyAttack()
{
	GetPlayerCharacter()->HeavyAttack();
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
		GetPlayerCharacter()->SetTargetRotation(FRotator::ZeroRotator);
		return;
	}

	GetPlayerCharacter()->SetTargetRotation(FVector(Value.X, Value.Y, 0).Rotation());
	isAiming = true;
}

void ACustomPlayerController::Shoot()
{
	if (GetPlayerCharacter()->GetEquippedRangedWeapon() && GetPlayerCharacter()->GetEquippedRangedWeapon()->GetAmmoInMagazine() <= 0 && ShouldReloadOnFire)
	{
		GetPlayerCharacter()->ReloadWeapon();
	}
	
	GetPlayerCharacter()->UseWeapon();
}

void ACustomPlayerController::ReleaseShoot()
{
	GetPlayerCharacter()->StopUsingWeapon();
}

void ACustomPlayerController::ReloadWeapon()
{
	GetPlayerCharacter()->ReloadWeapon();
}

void ACustomPlayerController::EquipNextWeapon()
{
	GetPlayerCharacter()->EquipNextWeapon();
}

void ACustomPlayerController::Interact()
{
	GetPlayerCharacter()->Interact();
}

// ------ GETTERS ------

APlayerCharacter* ACustomPlayerController::GetPlayerCharacter()
{
	if (PossessedPlayerCharacter == nullptr)
	{
		PossessedPlayerCharacter = Cast<APlayerCharacter>(GetCharacter());
	}

	return PossessedPlayerCharacter;
}
