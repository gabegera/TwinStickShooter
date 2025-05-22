// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "Camera/CameraComponent.h"
#include "Characters/CustomCharacter.h"
#include "PlayerCharacter.generated.h"

UENUM(BlueprintType)
enum EMeleeAction : uint8
{
	LightAttack = 0 UMETA(DisplayName = "Light Attack"),
	HeavyAttack = 1 UMETA(DisplayName = "Heavy Attack"),
};

USTRUCT(BlueprintType)
struct FMeleeAttack
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	TEnumAsByte<EMeleeAction> Action = LightAttack;

	UPROPERTY(EditAnywhere)
	UAnimMontage* AttackAnimation = nullptr;

	bool operator==(const FMeleeAttack& Other) const
	{
		return Action == Other.Action && AttackAnimation == Other.AttackAnimation;
	}
	bool operator!=(const FMeleeAttack& Other) const
	{
		return !(*this == Other);
	}
};

USTRUCT(BlueprintType)
struct FMeleeCombo
{
	GENERATED_BODY()

	UPROPERTY(EditAnywhere)
	FString ComboName = "Empty Combo Name";

	// These are the attacks and the order that is needed to perform the combo.
	UPROPERTY(EditAnywhere)
	TArray<FMeleeAttack> Attacks;

	bool operator==(const FMeleeCombo& Other) const
	{
		return ComboName == Other.ComboName && Attacks == Other.Attacks;
	}
	bool operator!=(const FMeleeCombo& Other) const
	{
		return !(*this == Other);
	}
};

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
	UStaticMeshComponent* MeleeWeapon;

	// ------ MELEE ------ //
	
	UPROPERTY(EditAnywhere, Category="Melee")
	TArray<FMeleeCombo> MeleeCombos;

	// Used to keep track of what combo is currently being used.
	TArray<EMeleeAction> ComboTracker;

	// This is the combo that the player is currently performing.
	FMeleeCombo CurrentActiveCombo;

	UPROPERTY(EditAnywhere)
	float ComboResetDelay = 1.0f;
	UPROPERTY() FTimerHandle ComboResetTimer;
	
	UPROPERTY(EditAnywhere)
	bool bShowDebugLogs = true;

public:

	UFUNCTION(BlueprintCallable)
	void LightAttack();

	UFUNCTION(BlueprintCallable)
	void HeavyAttack();

	// Using the combo tracker, finds which combo is currently being used.
	UFUNCTION()
	bool FindCombo();

	UFUNCTION(BlueprintCallable)
	void ResetCombo();
	
	virtual void Tick(float DeltaSeconds) override;
	
};
