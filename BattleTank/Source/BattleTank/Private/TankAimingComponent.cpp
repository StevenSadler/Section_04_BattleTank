// Fill out your copyright notice in the Description page of Project Settings.


#include "BattleTank.h"
#include "TankBarrel.h"
#include "TankAimingComponent.h"


// Sets default values for this component's properties
UTankAimingComponent::UTankAimingComponent()
{
	// Set this component to be initialized when the game starts, and to be ticked every frame.  You can turn these features
	// off to improve performance if you don't need them.
	PrimaryComponentTick.bCanEverTick = true;

	// ...
}

void UTankAimingComponent::SetBarrelReference(UTankBarrel* BarrelToSet)
{
	Barrel = BarrelToSet;
}

void UTankAimingComponent::AimAtWithLog(FVector HitLocation, float LaunchSpeed)
{
	AimAt(HitLocation, LaunchSpeed, true);
	//auto MyTankName = GetOwner()->GetName();
	//auto BarrelLocation = Barrel->GetComponentLocation();
	//UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s from %s"), *MyTankName, *HitLocation.ToString(), *BarrelLocation.ToString());
	//UE_LOG(LogTemp, Warning, TEXT("Firing at %f"), LaunchSpeed);
}

void UTankAimingComponent::AimAt(FVector HitLocation, float LaunchSpeed, bool log)
{
	if (!Barrel) { return; }

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
		auto AimDirection = TossVelocity.GetSafeNormal();
		auto TankName = GetOwner()->GetName();
		if (log)
		{
			UE_LOG(LogTemp, Warning, TEXT("%s aiming at %s"), *TankName, *AimDirection.ToString());
		}

		MoveBarrelTo(AimDirection);
	}
	else
	{
		auto Time = GetWorld()->GetTimeSeconds();
		UE_LOG(LogTemp, Warning, TEXT("%f: No aim solution found"), Time);
	}
}

void UTankAimingComponent::MoveBarrelTo(FVector AimDirection)
{
	// calculate difference between current barrel rotatio and aim direction
	auto BarrelRotator = Barrel->GetForwardVector().Rotation();
	auto AimRotator = AimDirection.Rotation();
	auto DeltaRotator = AimRotator - BarrelRotator;
	UE_LOG(LogTemp, Warning, TEXT("AimRotator: %s"), *AimRotator.ToString());

	// move the barrel the right amount this frame
	// given a max elevation speed and the frame time
	Barrel->Elevate(5); // TODO remove magic number
}
