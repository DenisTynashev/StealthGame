// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "Perception/PawnSensingComponent.h"
#include "DrawDebugHelpers.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
 	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>(TEXT("PawnSensingComp"));
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnSeenPawn);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnHearNoise);
}

void AFPSAIGuard::OnSeenPawn(APawn* SeenPawn)
{
	if (SeenPawn == nullptr) { return; }
	DrawDebugSphere(GetWorld(), SeenPawn->GetActorLocation(), 32.0f, 12, FColor::Cyan, false, 10.0f);
	UE_LOG(LogTemp, Warning, TEXT("I have see: %s"), *SeenPawn->GetName());
}

void AFPSAIGuard::OnHearNoise(APawn* Pawn, const FVector& Location, float Volume)
{
	if (Pawn == nullptr) { return; }
	DrawDebugSphere(GetWorld(),Location, 32.0f, 12, FColor::Red, false, 10.0f);
	UE_LOG(LogTemp, Warning, TEXT("I have hear: %s"), *Pawn->GetName());
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}


