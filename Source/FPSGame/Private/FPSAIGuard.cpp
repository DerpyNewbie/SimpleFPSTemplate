// Fill out your copyright notice in the Description page of Project Settings.


#include "FPSAIGuard.h"
#include "FPSGameMode.h"
#include "DrawDebugHelpers.h"
#include "Net/UnrealNetwork.h"
#include "Engine/World.h"
#include "Perception/PawnSensingComponent.h"
#include "TimerManager.h"

// Sets default values
AFPSAIGuard::AFPSAIGuard()
{
	// Set this character to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;

	// Setup sensing component
	PawnSensingComp = CreateDefaultSubobject<UPawnSensingComponent>("PawnSensingComp0");
	PawnSensingComp->OnSeePawn.AddDynamic(this, &AFPSAIGuard::OnPawnSeen);
	PawnSensingComp->OnHearNoise.AddDynamic(this, &AFPSAIGuard::OnNoiseHeard);

	// Default values
	bDrawSeenSphere = false;
	bDrawHeardSphere = false;
}

// Called when the game starts or when spawned
void AFPSAIGuard::BeginPlay()
{
	Super::BeginPlay();

	OriginalRotation = GetActorRotation();
}

void AFPSAIGuard::OnPawnSeen(APawn* SeenPawn)
{
	if (GuardState == EAIState::Alerted || SeenPawn == nullptr)
		return;

	UE_LOG(LogTemp, VeryVerbose, TEXT("an AI seen the pawn!"));

	if (bDrawSeenSphere)
		DrawDebugSphere(SeenPawn->GetWorld(), SeenPawn->GetActorLocation(), 32.f, 12, FColor::Red, false, 10.f);

	SetGuardState(EAIState::Alerted);
	AFPSGameMode* gm = Cast<AFPSGameMode>(GetWorld()->GetAuthGameMode());
	if (gm)
	{
		gm->CompleteMission(SeenPawn, false);
	}
}

void AFPSAIGuard::OnNoiseHeard(APawn * NoisyPawn, const FVector & Location, float Volume)
{
	if (GuardState == EAIState::Alerted)
		return;

	UE_LOG(LogTemp, VeryVerbose, TEXT("an AI heard the noise!"));

	if (bDrawHeardSphere)
		DrawDebugSphere(NoisyPawn->GetWorld(), Location, Volume * 32.f , 12, FColor::Emerald, false, 10.f);

	FVector direction = Location - GetActorLocation();
	direction.Normalize();

	FRotator newLookAt = FRotationMatrix::MakeFromX(direction).Rotator();
	newLookAt.Pitch = 0.f;
	newLookAt.Roll = 0.f;

	SetGuardState(EAIState::Suspicious);
	SetActorRotation(newLookAt);

	GetWorldTimerManager().SetTimer(TimerHandle, this, &AFPSAIGuard::ResetOrientation, 3.f);

}

void AFPSAIGuard::ResetOrientation()
{
	if (GuardState == EAIState::Alerted)
		return;

	UE_LOG(LogTemp, VeryVerbose, TEXT("resetting AI orientation!"));
	SetActorRotation(OriginalRotation);
	SetGuardState(EAIState::Idle);
}

void AFPSAIGuard::OnRep_GuardState()
{
	OnStateChange(GuardState);
}

// Called every frame
void AFPSAIGuard::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);

}

void AFPSAIGuard::SetGuardState(EAIState NewState)
{
	if (NewState == GuardState)
		return;
	if (GuardState == EAIState::Alerted)
		UE_LOG(LogTemp, Warning, TEXT("Changing AI State from Alerted!"));

	GuardState = NewState;
	OnRep_GuardState();

}

void AFPSAIGuard::GetLifetimeReplicatedProps(TArray<FLifetimeProperty > & OutLifetimeProps) const
{
	Super::GetLifetimeReplicatedProps(OutLifetimeProps);

	DOREPLIFETIME(AFPSAIGuard, GuardState);
}

