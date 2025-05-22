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
	TArray<AActor*> ActorsToIgnore;
	ActorsToIgnore.Add(GetInstigator());
	ActorsToIgnore.Add(Spawner);
	UKismetSystemLibrary::SphereTraceSingle(GetWorld(), PreviousLocation, GetActorLocation(), Sphere->GetScaledSphereRadius(),
		UEngineTypes::ConvertToTraceType(ECC_WorldDynamic), false, ActorsToIgnore,
		EDrawDebugTrace::ForOneFrame, Hit, true);

	PreviousLocation = GetActorLocation();
	
	if (Hit.GetActor())
	{
		// If collided with another projectile ignore it.
		if (Hit.GetActor()->GetClass() == GetClass()) return;
		
		Hit.GetActor()->TakeDamage(Damage, FPointDamageEvent(), GetInstigatorController(), GetInstigator());
		
		DestroyProjectile();
	}
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

