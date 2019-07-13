// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSGameState.h"
#include "FPSPlayerController.h"
#include "Engine/World.h"

void AFPSGameState::MulticastOnMissionComplete_Implementation(APawn* InstigatorPawn, bool bIsSucceed)
{
	//for (FConstPawnIterator i = GetWorld()->GetPawnIterator(); i; i++)
	//{
	//	APawn* pawn = i->Get();
	//	if (pawn || pawn->IsLocallyControlled())
	//	{
	//		pawn->DisableInput(nullptr);
	//		return;
	//	}
	//}
	UE_LOG(LogTemp, Verbpse, TEXT("Executing MulticastOnMissionComplete"));
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