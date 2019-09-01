// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "UObject/ConstructorHelpers.h"
#include "EngineUtils.h"
#include "SpectatingViewpoint.h"
#include "FPSGameState.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	//
	GameStateClass = AFPSGameState::StaticClass();
}

void AFPSGameMode::CompleteMission(APawn* InstigatorPawn, bool bMissionSuccess)
{
	if (InstigatorPawn)
	{
		/*TODO В лекции рассматривается вариант через GameStatics. 
		в Header необходимо тогда обьявить так
		UPROPERTY (EditDefaultsOnly)
		TSubclassOf<AActor> ClassToFind; // Needs to be populated somehow (e.g. by exposing to blueprints as uproperty and setting it there
		в cpp
		TArray<AActor*> FoundActors;
		UGameplayStatics::GetAllActorsOfClass(GetWorld(), ClassToFind, FoundActors);
		в blueprint
		присвоить ClassToFind значение искомого BP-класса
		*/
		APlayerController* ActualPlayerController = Cast<APlayerController>(InstigatorPawn->GetController());
		ASpectatingViewpoint* NewViewpoint = nullptr;
		for (TActorIterator<ASpectatingViewpoint> ActorItr(GetWorld()); ActorItr; ++ActorItr)
		{
			// Same as with the Object Iterator, access the subclass instance with the * or -> operators.
			NewViewpoint = *ActorItr;
			break;
		}
		if (NewViewpoint)
		{
			for (FConstPlayerControllerIterator It = InstigatorPawn->GetWorld()->GetPlayerControllerIterator(); It; It++)
			{
				APlayerController* PC = It->Get();
				PC->SetViewTargetWithBlend(NewViewpoint, 0.05f, EViewTargetBlendFunction::VTBlend_Cubic);
			}			
		}
		else
		{
			//TODO Fault message to log!
			UE_LOG(LogTemp, Warning, TEXT("No SpectatingViewpoint found!"));
		}
	}

	AFPSGameState* GS = GetGameState<AFPSGameState>();
	if (GS)
	{
		GS->MulticastOnMissionComplete(InstigatorPawn, bMissionSuccess);
	}
	OnMissionCompleted(InstigatorPawn, bMissionSuccess);
}
