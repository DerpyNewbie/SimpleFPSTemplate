// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"
#include "Engine/World.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bIsSucceed)
{

	UE_LOG(LogTemp, Verbose, TEXT("Executing MulticastOnMissionComplete"));
	for (FConstPlayerControllerIterator i = GetWorld()->GetPlayerControllerIterator(); i; i++)
	{
		AFPSPlayerController* pc = Cast<AFPSPlayerController>(i->Get());
		if (pc && pc->IsLocalController())
		{
			pc->OnMissionCompleted(InstigatorPawn, bIsSucceed);

			APawn* pawn = pc->GetPawn();
			if (pawn)
			{
				pawn->DisableInput(nullptr);
			}
		}
	}
}