// Fill out your copyright notice in the Description page of Project Settings.

#include "BattleTank.h"
#include "TankMovementComponent.h"

UTankMovementComponent::UTankMovementComponent()
{
	
}

void UTankMovementComponent::SetLeftTrackReference(UTankTrack* TrackToSet)
{
	if (!TrackToSet) { return; }
	LeftTrack = TrackToSet;
}

void UTankMovementComponent::SetRightTrackReference(UTankTrack* TrackToSet)
{
	if (!TrackToSet) { return; }
	RightTrack = TrackToSet;
}

void UTankMovementComponent::IntendMoveForward(float Throw)
{
	auto Name = GetName();
	UE_LOG(LogTemp, Warning, TEXT("Intend move forward throw: %f"), Throw);
}


