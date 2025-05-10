// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Actors/Weapon.h"
#include "GameFramework/Character.h"
#include "CustomCharacter.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API ACustomCharacter : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	ACustomCharacter();

protected:
	
	UPROPERTY(EditAnywhere)
	TArray<TSubclassOf<AWeapon>> Weapons;

	UPROPERTY(EditAnywhere)
	int32 MaxWeaponCapacity = 2;

	UPROPERTY()
	AWeapon* EquippedWeapon = nullptr;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	UFUNCTION(BlueprintCallable)
	void UseWeapon();

	UFUNCTION(BlueprintCallable)
	void StopUsingWeapon();

	UFUNCTION(BlueprintCallable)
	void EquipNextWeapon();
	
	void EquipWeapon(int32 Index);

	UFUNCTION(BlueprintCallable)
	void EquipWeapon(TSubclassOf<AWeapon> WeaponClass);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};
