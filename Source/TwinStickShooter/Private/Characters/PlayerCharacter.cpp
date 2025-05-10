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
}
