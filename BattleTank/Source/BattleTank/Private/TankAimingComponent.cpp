// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTank.h"
#include "TankTurret.h"
#include "TankBarrel.h"
#include "Projectile.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::TickComponent(float DeltaTime, enum ELevelTick TickType, FActorComponentTickFunction *ThisTickFunction)
{
	double CurrentTime = GetWorld()->GetTimeSeconds();
	if ((CurrentTime - LastFireTime) < ReloadTimeInSeconds)
	{
		FiringState = EFiringState::Loading;
	}
	else if (IsBarrelMoving())
	{
		FiringState = EFiringState::Aiming;
	}
	else
	{
		FiringState = EFiringState::Ready;
	}
	// TODO handle aiming and ready states
}

bool UTankAimingComponent::IsBarrelMoving()
{
	if (!ensure(Barrel)) { return false; }
	auto BarrelForward = Barrel->GetForwardVector();
	return !BarrelForward.Equals(AimDirection, 0.01);
}

void UTankAimingComponent::Initialize(UTankBarrel* BarrelToSet, UTankTurret* TurretToSet)
{
	Barrel = BarrelToSet;
	Turret = TurretToSet;
}

void UTankAimingComponent::AimAt(FVector HitLocation)
{
	if (!ensure(Barrel)) { return; }

	FVector TossVelocity;
	FVector StartLocation = Barrel->GetSocketLocation(FName("Projectile"));

	// calculate the OutLaunchVelocity
	bool bHasAimSolution = UGameplayStatics::SuggestProjectileVelocity(
		this,
		TossVelocity,
		StartLocation,
		HitLocation,
		LaunchSpeed,
		false,
		0.0f,
		0.0f,
		ESuggestProjVelocityTraceOption::DoNotTrace
	);

	if (bHasAimSolution)
	{
		AimDirection = TossVelocity.GetSafeNormal();
		MoveBarrelTo(AimDirection);
	}
}

void UTankAimingComponent::MoveBarrelTo(FVector AimDirection)
{
	if (!ensure(Barrel) || !ensure(Turret)) { return; }

	// calculate difference between current barrel rotatio and aim direction
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimRotator = AimDirection.Rotation();
	auto DeltaRotator = AimRotator - BarrelRotator;

	Barrel->Elevate(DeltaRotator.Pitch); // TODO remove magic number
	Turret->Rotate(DeltaRotator.Yaw);
}

void UTankAimingComponent::Fire()
{
	double CurrentTime = GetWorld()->GetTimeSeconds();
	bool isLoaded = (CurrentTime - LastFireTime) > ReloadTimeInSeconds;
	if (FiringState != EFiringState::Loading)
	{
		if (!ensure(Barrel)) { return; }
		if (!ensure(ProjectileBlueprint)) { return; }

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
