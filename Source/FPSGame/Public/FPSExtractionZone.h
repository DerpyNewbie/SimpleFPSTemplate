// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSExtractionZone.generated.h"

UCLASS()
class FPSGAME_API AFPSExtractionZone : public AActor
{
	GENERATED_BODY()

protected:

	UPROPERTY(EditDefaultsOnly, Category = "Extraction Zone")
		class UBoxComponent* OverlapComp;

	UPROPERTY(EditDefaultsOnly, Category = "Extraction Zone")
		class UDecalComponent* DecalComp;

	UPROPERTY(EditDefaultsOnly, Category = "Extraction Zone")
		class USoundBase* MissingObjectiveSE;

public:
	// Sets default values for this actor's properties
	AFPSExtractionZone();

protected:

	UFUNCTION()
	void HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

public:


};
