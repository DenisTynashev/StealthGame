// Fill out your copyright notice in the Description page of Project Settings.


#include "GuardAIController.h"
#include "Engine/TargetPoint.h"
#include "Kismet/GameplayStatics.h"
#include "Engine/World.h"
#include "Classes/Navigation/PathFollowingComponent.h"
#include "AIController.h"
#include "FPSAIGuard.h"

void AGuardAIController::OnGuardStateChanged(EAIState ActualGuardState)
{
	UE_LOG(LogTemp, Warning, TEXT("New State is %d"), ActualGuardState);
	if (!ControlledGuard) { return; }
	if (ActualGuardState == EAIState::Alerted
		|| ActualGuardState == EAIState::Suspicious)
	{
		UE_LOG(LogTemp, Warning, TEXT("Try to stop movement"));
		StopMovement();
	}
	else if (ActualGuardState == EAIState::Idle)
	{
		int result = MoveToActor(Waypoints[WaypointIndex]);
	}
}

void AGuardAIController::BeginPlay()
{
	Super::BeginPlay();
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	WaypointIndex = 0;
	UGameplayStatics::GetAllActorsOfClass(GetWorld(),ATargetPoint::StaticClass(), Waypoints);
	UE_LOG(LogTemp, Warning, TEXT("Now we are in  AGuardAIController::BeginPlay()"));
	UE_LOG(LogTemp, Warning, TEXT("Numer of waypoints found: %d"), Waypoints.Num());
	int result = MoveToActor(Waypoints[WaypointIndex]);
	UE_LOG(LogTemp, Warning, TEXT("Results of moving %d"), result);
	ControlledGuard= Cast<AFPSAIGuard>(AController::GetPawn());
	ControlledGuard->GuadrStateChanged.AddDynamic(this, &AGuardAIController::OnGuardStateChanged);
	
}

void AGuardAIController::OnMoveCompleted(FAIRequestID RequestID, const FPathFollowingResult& Result)
{
	Super::OnMoveCompleted(RequestID, Result);
	UE_LOG(LogTemp, Warning, TEXT("Moving is completed"));
	if (ControlledGuard->GetGuardState() == EAIState::Idle)
	{
		if (WaypointIndex < Waypoints.Num() - 1)
		{
			WaypointIndex++;
		}
		else
		{
			WaypointIndex = 0;
		}
		int result = MoveToActor(Waypoints[WaypointIndex]);
	}	
}

void AGuardAIController::Tick(float DeltaSeconds)
{
	Super::Tick(DeltaSeconds);
}
