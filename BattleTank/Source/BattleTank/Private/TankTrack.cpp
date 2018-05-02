// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankTrack.h"

void UTankTrack::SetThrottle(float Throttle)
{
	Throttle = FMath::Clamp(Throttle, -1.0f, 1.0f);
	auto Time = GetWorld()->GetTimeSeconds();
	auto Name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("%s throttle: %f"), *Name, Throttle);

	// clamp actual throttle so player cannot overdrive
	auto ForceApplied = GetForwardVector() * Throttle * TrackMaxDrivingForce;
	auto ForceLocation = GetComponentLocation();

	auto TankRoot = Cast<UPrimitiveComponent>(GetOwner()->GetRootComponent());
	TankRoot->AddForceAtLocation(ForceApplied, ForceLocation);
}


