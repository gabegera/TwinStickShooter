// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Projectile.h"
#include "Components/ArrowComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/Character.h"
#include "Weapon.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API AWeapon : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AWeapon();

protected:

	UPROPERTY(EditAnywhere)
	USkeletalMeshComponent* SkeletalMesh;

	// ------ WEAPON VARIABLES ------

	UPROPERTY(EditAnywhere, Category="Weapon")
	FString DisplayName = "Unnamed Weapon";
	
	UPROPERTY(EditAnywhere, Category="Weapon")
	float Damage = 10.0f;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:

	// Triggers the weapons attack.
	UFUNCTION(BlueprintCallable)
	virtual void Fire();

	UFUNCTION(BlueprintCallable)
	virtual void ReleaseFire();

	UFUNCTION(BlueprintCallable)
	void SetWeaponCollision(bool Input);
	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USkeletalMeshComponent* GetSkeletalMesh() const { return SkeletalMesh; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	FString GetDisplayName() { return DisplayName; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	bool CanBePickedUp() { return !GetAttachParentActor(); }
};
