// Fill out your copyright notice in the Description page of Project Settings.


#include "SpectatingViewpoint.h"

// Sets default values
ASpectatingViewpoint::ASpectatingViewpoint()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp"));	
	SetRootComponent(MeshComp);
	MeshComp->SetHiddenInGame(true, true);
}

// Called when the game starts or when spawned
void ASpectatingViewpoint::BeginPlay()
{
	Super::BeginPlay();
	
}

// Called every frame
void ASpectatingViewpoint::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

