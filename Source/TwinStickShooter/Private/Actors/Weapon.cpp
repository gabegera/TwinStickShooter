// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Weapon.h"

// Sets default values
AWeapon::AWeapon()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("Root"));

	SkeletalMesh = CreateDefaultSubobject<USkeletalMeshComponent>(TEXT("SkeletalMesh"));
	SkeletalMesh->SetupAttachment(RootComponent);
	SkeletalMesh->SetCollisionProfileName("Weapon");
	SkeletalMesh->SetGenerateOverlapEvents(true);
	SkeletalMesh->CanCharacterStepUpOn = ECB_No;
}

// Called when the game starts or when spawned
void AWeapon::BeginPlay()
{
	Super::BeginPlay();
}

void AWeapon::Fire()
{
	
}

void AWeapon::ReleaseFire()
{
	
}

void AWeapon::SetWeaponCollision(bool Input)
{
	if (Input == true)
	{
		SkeletalMesh->SetCollisionProfileName("Weapon");
		SkeletalMesh->SetSimulatePhysics(true);
		SkeletalMesh->SetGenerateOverlapEvents(true);
	}
	else
	{
		SkeletalMesh->SetSimulatePhysics(false);
		SkeletalMesh->SetCollisionProfileName("NoCollision");
		SkeletalMesh->SetGenerateOverlapEvents(false);
	}
}

// Called every frame
void AWeapon::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
}

