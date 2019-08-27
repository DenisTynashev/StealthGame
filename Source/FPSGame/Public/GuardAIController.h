// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "AIController.h"
#include "GuardAIController.generated.h"

class AFPSAIGuard;

/**
 * 
 */
UCLASS()
class FPSGAME_API AGuardAIController : public AAIController
{
	GENERATED_BODY()

protected:
	UPROPERTY(EditDefaultsOnly)
		TArray<AActor*> Waypoints;

	int32 WaypointIndex;
	
	bool bIsMovementAllowed;

	AFPSAIGuard* ControlledGuard;

	UFUNCTION()
	void OnGuardStateChanged(EAIState ActualGuardState);

	

private:
	virtual void BeginPlay() override;

	virtual void OnMoveCompleted (FAIRequestID RequestID, const FPathFollowingResult& Result) override;

	virtual void Tick(float DeltaSeconds) override;
	
};
