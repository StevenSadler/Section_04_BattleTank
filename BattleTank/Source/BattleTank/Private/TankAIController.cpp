// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "Tank.h"
#include "TankAIController.h"

void ATankAIController::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

	auto PlayerTank = Cast<ATank>(GetWorld()->GetFirstPlayerController()->GetPawn());
	auto MyTank = Cast<ATank>(GetPawn());
	if (ensure(PlayerTank))
	{
		// move toward the player
		MoveToActor(PlayerTank, AcceptanceRadius);

		// aim towards the player
		MyTank->AimAt(PlayerTank->GetActorLocation());

		// fire if ready
		MyTank->Fire(); // TODO limit firing rate
	}
}

