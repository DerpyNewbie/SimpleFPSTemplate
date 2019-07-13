// Fill out your copyright notice in the Description page of Project Settings.

#include "FPSLaunchPad.h"
#include "TimerManager.h"
#include "DrawDebugHelpers.h"
#include "GameFramework/Character.h"
#include "Kismet/GameplayStatics.h"
#include "Components/PrimitiveComponent.h"
#include "Components/BoxComponent.h"
#include "Components/StaticMeshComponent.h"

// Sets default values
AFPSLaunchPad::AFPSLaunchPad()
{
	BoxComp = CreateDefaultSubobject<UBoxComponent>(TEXT("BoxComp0"));
	BoxComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	BoxComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	BoxComp->SetBoxExtent(FVector(80.0f));
	RootComponent = BoxComp;

	BasePlateComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("BasePlate0"));
	BasePlateComp->SetupAttachment(BoxComp);

	ArrowTextureComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("ArrowTexture0"));
	ArrowTextureComp->SetupAttachment(BoxComp);

	if (!bUseLegacy) {
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::HandleBeginOverlap);
	}
	else {
		BoxComp->OnComponentBeginOverlap.AddDynamic(this, &AFPSLaunchPad::LegacyHandleBeginOverlap);
	}

	// Default values
	LaunchForce = 1000;
	LaunchPitch = 45;
	LaunchYaw = 0;
	LaunchRoll = 0;
	bUseLaunchCharacter = true;
	bDynamicVelocityCalculation = true;
	bDrawDebugLine = true;
}

void AFPSLaunchPad::BeginPlay()
{
	Super::BeginPlay();

	calculateVelocity();
}

void AFPSLaunchPad::HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Executed overlap handler!"));

	// Shouldn't do anything to own actor.
	if (OtherActor == this)
	{
		UE_LOG(LogTemp, Warning, TEXT("Found owning actor."));
		return;
	}

	// Calculate velocity every overlap event.
	if (bDynamicVelocityCalculation)
	{
		UE_LOG(LogTemp, Warning, TEXT("Dynamic velocity calculation is enabled!"));
		calculateVelocity();
	}
	UE_LOG(LogTemp, Verbose, TEXT("Current VelocityVector: %g, %g, %g"), VelocityVector.X, VelocityVector.Y, VelocityVector.Z);

	// Draw an debug line to expected velocity location.
	if (bDrawDebugLine)
		DrawDebugLine(GetWorld(), OtherActor->GetActorLocation(), OtherActor->GetActorLocation() + VelocityVector, FColor(uint8(255), uint8(0), uint8(0)), false, 10.f);

	// Check for character
	ACharacter* tempChara = Cast<ACharacter>(OtherActor);
	if (tempChara && bUseLaunchCharacter)
	{
		UE_LOG(LogTemp, VeryVerbose, TEXT("Found Character"));
		tempChara->LaunchCharacter(VelocityVector, bPreventOtherActorVelocity, bPreventOtherActorVelocity);
	}
	// Check for UPrimitiveComponent
	else if (OtherComp && (bCheckSimlatingPhysics ? OtherComp->IsSimulatingPhysics() : true))
	{
		UE_LOG(LogTemp, VeryVerbose, TEXT("Found Primitive Component"));
		OtherComp->AddImpulse(VelocityVector, FName("None"), bPreventOtherActorVelocity);
	}
	// Catch fail
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to launch an actor."));
		return;
	}

	playEffect();
}

void AFPSLaunchPad::LegacyHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent,
	AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult)
{
	UE_LOG(LogTemp, Log, TEXT("Executed overlap handler! (legacy)"));

	// If OtherActor is myself, then just return.
	if (OtherActor == this)
	{
		UE_LOG(LogTemp, Verbose, TEXT("found self"));
		return;
	}

	// If OtherActor is ACharactor, then launch them.
	ACharacter* tempChara = Cast<ACharacter>(OtherActor);
	if (tempChara)
	{
		UE_LOG(LogTemp, VeryVerbose, TEXT("found character"));
		tempChara->LaunchCharacter(LegacyVelocityLocation + (tempChara->GetVelocity() * -1), false, false);
	}
	// If UPrimitiveComponent exists, then add impulse to UPrimitiveComponent. (if bCheckSimulatingPhysics are true, it will add check for Simulating physics or not.)
	else if (OtherComp && (bCheckSimlatingPhysics ? OtherComp->IsSimulatingPhysics() : true))
	{
		UE_LOG(LogTemp, VeryVerbose, TEXT("found primitive component"));
		OtherComp->AddImpulse(LegacyVelocityLocation + (OtherComp->GetComponentVelocity() * -1), FName("None"), true);
	}
	// If failed above two ifs, then log the error and return.
	else
	{
		UE_LOG(LogTemp, Error, TEXT("Failed to launch an actor."));
		return;
	}

	// Play particle effect if launching has succeed.
	playEffect();
}

void AFPSLaunchPad::playEffect()
{
	UE_LOG(LogTemp, Log, TEXT("Spawning particle effect!"));
	UGameplayStatics::SpawnEmitterAtLocation(this, ParticleEffect, GetActorLocation());
}

bool AFPSLaunchPad::primCompCheck(UPrimitiveComponent* primComp)
{
	if (bCheckSimlatingPhysics)
		return primComp->IsSimulatingPhysics();
	else return true;
}


/*

	Velocity's origin point is NOT UWorld's origin point <= i used 2 days to figure out this

*/
void AFPSLaunchPad::calculateVelocity()
{
	// Log stuff
	UE_LOG(LogTemp, Log, TEXT("Calculating velocity!"));

	if (LaunchPitch == 0 || LaunchForce == 0)
		UE_LOG(LogTemp, Warning, TEXT("LaunchAngle or LaunchForce is set to 0, this will might not work correctly."));
	UE_LOG(LogTemp, VeryVerbose, TEXT("LaunchForce: %g, LaunchAngle: %g, Yaw: %g, Roll: %g"), LaunchForce, LaunchPitch, LaunchYaw, LaunchRoll);

	// Setup velocity
	FRotator baseRotator = GetActorRotation() + FRotator(LaunchPitch, LaunchYaw, LaunchRoll);
	UE_LOG(LogTemp, VeryVerbose, TEXT("Rotator: P:%g, Y:%g, R:%g"), baseRotator.Pitch, baseRotator.Yaw, baseRotator.Roll);

	VelocityVector = baseRotator.RotateVector(FVector(LaunchForce, 0.f, 0.f));
	UE_LOG(LogTemp, VeryVerbose, TEXT("Velocity: %g, %g, %g"), VelocityVector.X, VelocityVector.Y, VelocityVector.Z);

	UE_LOG(LogTemp, Log, TEXT("Calculation completed!"));
	if (bDrawDebugLine)
		DrawDebugLine(GetWorld(), GetActorLocation(), GetActorLocation() + VelocityVector, FColor(uint8(0), uint8(255), uint8(0)), false, 10.f);
}