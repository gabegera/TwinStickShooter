// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/PlayerController.h"
#include "EnhancedInputSubsystems.h"
#include "InputMappingContext.h"
#include "EnhancedInputComponent.h"
#include "Characters/PlayerCharacter.h"
#include "CustomPlayerController.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API ACustomPlayerController : public APlayerController
{
	GENERATED_BODY()

protected:
	
	UPROPERTY() APlayerCharacter* PossessedPlayerCharacter = nullptr;

	// ------ INPUT ------
	
	// Default Mapping Context used for Enhanced Input.
	UPROPERTY(EditAnywhere, BlueprintReadOnly, Category = "Input")
	TSoftObjectPtr<UInputMappingContext> InputMapping;

	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_Move;
	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_Dodge;
	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_LightAttack;
	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_HeavyAttack;
	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_Parry;
	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_Aim;
	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_Shoot;
	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_Reload;
	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_NextWeapon;
	UPROPERTY(EditAnywhere, Category = "Input") UInputAction* Input_Interact;

	// Between 0 and 1 how much the input is being triggered.
	float MovementInputPercentage = 0.0f;
	FVector MovementDirection = FVector::ZeroVector;

	bool isAiming = false;
	FRotator TargetAimRotation;

	UPROPERTY(EditAnywhere)
	float MaxRotationSpeed;

	// If true, when the weapon's magazine is out of ammo it will reload when you try to fire the weapon.
	UPROPERTY(EditAnywhere, Category = "Input")
	bool ShouldReloadOnFire = true;
	
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable, Category="Input")
	void SetupPlayerInputComponent(UInputComponent* PlayerInputComponent);

public:

	void Move(const FInputActionInstance& Instance);

	void Dodge();

	void LightAttack();

	void HeavyAttack();

	void Parry();

	void Aim(const FInputActionInstance& Instance);

	void Shoot();

	void ReleaseShoot();

	void ReloadWeapon();

	void EquipNextWeapon();

	void Interact();


	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure, Category = "Getters")
	APlayerCharacter* GetPlayerCharacter();
};
