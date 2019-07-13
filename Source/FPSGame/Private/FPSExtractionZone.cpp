// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSExtractionZone.h"
#include "FPSCharacter.h"
#include "FPSGameMode.h"
#include "Kismet/GameplayStatics.h"
#include "Components/BoxComponent.h"
#include "Components/DecalComponent.h"

// Sets default values
AFPSExtractionZone::AFPSExtractionZone()
{
	const FVector initsize = FVector(200.f);

	OverlapComp = CreateDefaultSubobject<UBoxComponent>("OverlapComp0");
	OverlapComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	OverlapComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Ignore);
	OverlapComp->SetCollisionResponseToChannel(ECollisionChannel::ECC_Pawn, ECollisionResponse::ECR_Overlap);
	OverlapComp->SetBoxExtent(initsize);
	RootComponent = OverlapComp;

	DecalComp = CreateDefaultSubobject<UDecalComponent>("DecalComp0");
	DecalComp->DecalSize = initsize;
	DecalComp->SetupAttachment(OverlapComp);

	OverlapComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSExtractionZone::HandleOverlap);
}

void AFPSExtractionZone::HandleOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	AFPSCharacter* myChara = Cast<AFPSCharacter>(OtherActor);
	if (myChara == nullptr)
	{
		return;
	}

	if (myChara->bIsCarryingObject)
	{
		AFPSGameMode* gm = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
		if (gm)
		{
			gm->CompleteMission(myChara);
		}
		else
		{
			UE_LOG(LogTemp, Error, TEXT("Failed to cast Auth GameMode as AFPSGameMode."));
		}
	}
	else
	{
		UGameplayStatics::PlaySound2D(myChara, MissingObjectiveSE);
	}


}


