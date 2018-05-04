// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "Projectile.h"
#include "TankBarrel.h"

// Sets default values
ATank::ATank()
{
 	// Set this pawn to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = false;
}

void ATank::BeginPlay()
{
	Super::BeginPlay();  // needed for BP BeginPlay to run
}

void ATank::Fire()
{
	if (!ensure(Barrel)) { return; }
	double CurrentTime = GetWorld()->GetTimeSeconds();
	bool isLoaded = (CurrentTime - LastFireTime) > ReloadTimeInSeconds;
	if (isLoaded)
	{
		// spawn a projectile at the socket location on the barrel
		auto Projectile = GetWorld()->SpawnActor<AProjectile>(
			ProjectileBlueprint,
			Barrel->GetSocketLocation(FName("Projectile")),
			Barrel->GetSocketRotation(FName("Projectile"))
			);

		Projectile->LaunchProjectile(LaunchSpeed);
		LastFireTime = CurrentTime;
	}
}


