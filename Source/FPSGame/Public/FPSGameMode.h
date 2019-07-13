// Copyright 1998-2017 Epic Games, Inc. All Rights Reserved.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/GameModeBase.h"
#include "FPSGameMode.generated.h"

UCLASS()
class AFPSGameMode : public AGameModeBase
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Spectator")
		TSubclassOf<AActor> SpectatorViewPointClass;

public:

	AFPSGameMode();

	void CompleteMission(APawn* InstigatorPawn, bool bIsSucceed = true);

	UFUNCTION(BlueprintImplementableEvent, Category = "GameMode")
		void OnMissionCompleted(APawn* InstigatorPawn, bool bIsSucceed);
};



