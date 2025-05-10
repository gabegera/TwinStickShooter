// Fill out your copyright notice in the Description page of Project Settings.


#include "Actors/Projectile.h"

#include "KismetTraceUtils.h"

// Sets default values
AProjectile::AProjectile()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	RootComponent = CreateDefaultSubobject<USceneComponent>(TEXT("RootComponent"));

	Mesh = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("Mesh"));
	Mesh->SetupAttachment(RootComponent);

	Sphere = CreateDefaultSubobject<USphereComponent>(TEXT("Sphere"));
	Sphere->SetupAttachment(Mesh);

	ProjectileMovement = CreateDefaultSubobject<UProjectileMovementComponent>(TEXT("ProjectileMovement"));
	ProjectileMovement->ProjectileGravityScale = 0;
	ProjectileMovement->bRotationFollowsVelocity = true;
		
}

// Called when the game starts or when spawned
void AProjectile::BeginPlay()
{
	Super::BeginPlay();
	
	GetWorldTimerManager().SetTimer(ProjectileDestructionTimer, this, &AProjectile::DestroyProjectile, ProjectileLifetime, false);
}

void AProjectile::CheckCollision()
{
	if (PreviousLocation == FVector::ZeroVector) PreviousLocation = GetActorLocation();
	
	FHitResult Hit;
	DrawDebugSphereTraceSingle(GetWorld(), PreviousLocation, GetActorLocation(), Sphere->GetScaledSphereRadius(), EDrawDebugTrace::ForOneFrame, true,
							   Hit, FColor::Red, FColor::Green, 0);

	PreviousLocation = GetActorLocation();

	// if (Hit.GetActor()->GetClass() == GetClass()) return;
	//
	// if (Hit.bBlockingHit) DestroyProjectile();
}

void AProjectile::DestroyProjectile()
{
	Destroy();
}

// Called every frame
void AProjectile::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	CheckCollision();
}

