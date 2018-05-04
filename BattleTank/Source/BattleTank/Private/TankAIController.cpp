// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankAimingComponent.h"
#include "TankAIController.h"

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = GetWorld()->GetFirstPlayerController()->GetPawn();
	auto MyTank = GetPawn();
	if (!ensure(PlayerTank && MyTank)) { return; }

	// move toward the player
	MoveToActor(PlayerTank, AcceptanceRadius);

	// aim towards the player
	auto AimingComponent = MyTank->FindComponentByClass<UTankAimingComponent>();
	AimingComponent->AimAt(PlayerTank->GetActorLocation());

	// fire if ready
	AimingComponent->Fire(); // TODO limit firing rate
}

