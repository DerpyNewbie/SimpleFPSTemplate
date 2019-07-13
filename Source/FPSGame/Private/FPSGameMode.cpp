// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#include "FPSGameMode.h"
#include "FPSGameState.h"
#include "FPSHUD.h"
#include "FPSCharacter.h"
#include "Engine/World.h"
#include "Kismet/GameplayStatics.h"
#include "UObject/ConstructorHelpers.h"

AFPSGameMode::AFPSGameMode()
{
	// set default pawn class to our Blueprinted character
	static ConstructorHelpers::FClassFinder<APawn> PlayerPawnClassFinder(TEXT("/Game/Blueprints/BP_Player"));
	DefaultPawnClass = PlayerPawnClassFinder.Class;

	// use our custom HUD class
	HUDClass = AFPSHUD::StaticClass();
	GameStateClass = AFPSGameState::StaticClass();

}

void AFPSGameMode::CompleteMission(APawn * InstigatorPawn, bool bIsSucceed)
{
	if (!InstigatorPawn)
	{
		return;
	}

	//InstigatorPawn->DisableInput(nullptr);

	if (SpectatorViewPointClass)
	{
		TArray<AActor*> viewPointActors;
		UGameplayStatics::GetAllActorsOfClass(this, SpectatorViewPointClass, viewPointActors);

		if (viewPointActors.Num() > 0)
		{
			AActor* viewPoint = viewPointActors[0];

			for (FConstPlayerControllerIterator i = GetWorld()->GetPlayerControllerIterator(); i; i++)
			{
				APlayerController* pc = i->Get();
				if (pc)
				{
					pc->SetViewTargetWithBlend(viewPoint, .5f, EViewTargetBlendFunction::VTBlend_Cubic);
				}
			}
		}
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Spectator View Point Class in FPSGameMode is nullptr!"));
	}

	AFPSGameState* gs = GetGameState<AFPSGameState>();
	if (gs)
	{
		gs->MulticastOnMissionComplete(InstigatorPawn, bIsSucceed);
	}
	else
	{
		UE_LOG(LogTemp, Warning, TEXT("Failed to cast GameState as AFPSGameState."));
	}
	OnMissionCompleted(InstigatorPawn, bIsSucceed);
}
