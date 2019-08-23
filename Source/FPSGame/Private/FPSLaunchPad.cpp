// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSLaunchPad.h"
#include "FPSCharacter.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"
#include "Kismet/GameplayStatics.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
 	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;	
	
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(75.0f, 75.0f, 50.0f));
	OverlapComp->SetHiddenInGame(true);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OnOverlapBegin);
	
	SetRootComponent(OverlapComp);
	PadComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadComp"));
	PadComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);
	PadComp->AttachToComponent(OverlapComp, FAttachmentTransformRules::KeepRelativeTransform);

	LaunchPitchAngle = 35.0f;
	LaunchStrength = 1500.0f;
}

// Called when the game starts or when spawned
void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();
	
}

void AFPSLaunchPad::PlayEffects()
{
	UGameplayStatics::SpawnEmitterAtLocation(GetWorld(), LaunchFX, GetActorLocation()); //instead of GetWorld() can be "this"
}

// Called every frame
void AFPSLaunchPad::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSLaunchPad::OnOverlapBegin(UPrimitiveComponent* OverlappedComp, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
	FRotator LaunchDirection = GetActorRotation();
	LaunchDirection.Pitch += LaunchPitchAngle;
	FVector LaunchVelocity = LaunchDirection.Vector() * LaunchStrength;

	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn != nullptr) 
	{
		MyPawn->LaunchCharacter(LaunchVelocity, true, true);
		PlayEffects();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
		UE_LOG(LogTemp, Warning, TEXT("OtherComp is: %s"), *OtherComp->GetName());
		UE_LOG(LogTemp, Warning, TEXT("This is: %s"), *this->GetName());
		if (OtherComp && OtherComp->IsSimulatingPhysics())
		{
			UE_LOG(LogTemp, Warning, TEXT("Add force!"));
			OtherComp->AddImpulse(LaunchVelocity, NAME_None, true);
			PlayEffects();
		}		
	}
	
}

