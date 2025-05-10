// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Components/SphereComponent.h"
#include "GameFramework/Actor.h"
#include "GameFramework/ProjectileMovementComponent.h"
#include "Projectile.generated.h"

UCLASS()
class TWINSTICKSHOOTER_API AProjectile : public AActor
{
	GENERATED_BODY()
	
public:	
	// Sets default values for this actor's properties
	AProjectile();

protected:

	UPROPERTY(EditAnywhere)
	UStaticMeshComponent* Mesh;

	UPROPERTY(EditAnywhere)
	USphereComponent* Sphere;

	UPROPERTY(EditAnywhere)
	UProjectileMovementComponent* ProjectileMovement;

	UPROPERTY()
	float Damage;

	UPROPERTY(EditAnywhere)
	float ProjectileLifetime = 8.0f;
	FTimerHandle ProjectileDestructionTimer;

	UPROPERTY()
	FVector PreviousLocation = FVector::ZeroVector;
	
	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION(BlueprintCallable)
	void CheckCollision();

	UFUNCTION(BlueprintCallable)
	void DestroyProjectile();

public:	
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	// ------ GETTERS ------

	UFUNCTION(BlueprintCallable, BlueprintPure)
	UStaticMeshComponent* GetMesh() const { return Mesh; }
	
	UFUNCTION(BlueprintCallable, BlueprintPure)
	UProjectileMovementComponent* GetProjectileMovement() const { return ProjectileMovement; }

	UFUNCTION(BlueprintCallable, BlueprintPure)
	USphereComponent* GetSphere() const { return Sphere; }

	// ------ SETTERS ------

	UFUNCTION(BlueprintCallable)
	float SetDamage(const float NewDamage) { return Damage = NewDamage; }

};
