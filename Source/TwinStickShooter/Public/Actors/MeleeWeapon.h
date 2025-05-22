// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Weapon.h"
#include "MeleeWeapon.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API AMeleeWeapon : public AWeapon
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AMeleeWeapon();
	
protected:
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	virtual void Fire() override;
	virtual void ReleaseFire() override;

	void LightAttack();

	void HeavyAttack();

	void ResetCombo();
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	// UFUNCTION(BlueprintCallable, BlueprintPure)
	// TArray<EMeleeAction>& GetComboTracker() { return ComboTracker; }
};
