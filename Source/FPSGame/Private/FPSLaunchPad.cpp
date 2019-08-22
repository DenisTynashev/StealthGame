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
	PadComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("PadComp"));
	PadComp->SetCollisionEnabled(ECollisionEnabled::NoCollision);	
	//OverlapComp->SetBoxExtent(FVector(100.0f));
	SetRootComponent(PadComp);
	
	OverlapComp = CreateDefaultSubobject<UBoxComponent>(TEXT("OverlapComp"));
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	OverlapComp->SetBoxExtent(FVector(100.0f));
	OverlapComp->SetHiddenInGame(false);
	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::OnOverlapBegin);
	OverlapComp->AttachToComponent(PadComp, FAttachmentTransformRules::KeepRelativeTransform);

	/*DecalComp = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp"));
	DecalComp->DecalSize = FVector(100.0f, 100.0f, 0.1f);
	DecalComp->AttachToComponent(PadComp, FAttachmentTransformRules::KeepRelativeTransform);

	DecalComp2 = CreateDefaultSubobject<UDecalComponent>(TEXT("DecalComp2"));
	DecalComp2->DecalSize = FVector(100.0f, 100.0f, 0.1f);
	DecalComp2->AttachToComponent(PadComp, FAttachmentTransformRules::KeepRelativeTransform);
	*/

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
	AFPSCharacter* MyPawn = Cast<AFPSCharacter>(OtherActor);
	if (MyPawn != nullptr) 
	{
		FVector LaunchVelocity = { 100.f, 0.f, 1000.0f };
		MyPawn->LaunchCharacter(LaunchVelocity, true, true);
		PlayEffects();
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Overlapped"));
		UE_LOG(LogTemp, Warning, TEXT("OtherComp is: %s"), *OtherComp->GetName());
		UE_LOG(LogTemp, Warning, TEXT("This is: %s"), *this->GetName());
		if (//OtherComp->GetOwner() != this
			//&& 
			OtherComp->IsSimulatingPhysics())
		{
			UE_LOG(LogTemp, Warning, TEXT("Add force!"));
			OtherComp->AddRadialForce(this->GetActorLocation(), 1000, 150000.0f, ERadialImpulseFalloff::RIF_Constant, true);			
			PlayEffects();
		}		
	}
	
}

