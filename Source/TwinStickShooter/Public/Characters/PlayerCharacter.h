// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Characters/CustomCharacter.h"
#include "PlayerCharacter.generated.h"


UCLASS()
class TWINSTICKSHOOTER_API APlayerCharacter : public ACustomCharacter
{
	GENERATED_BODY()

public:

	APlayerCharacter();

protected:

	UPROPERTY(EditAnywhere, BlueprintReadOnly)
	UCameraComponent* Camera;

	UPROPERTY(EditAnywhere)
	bool bShowDebugLogs = true;

public:

	virtual void Tick(float DeltaSeconds) override;
	
};
