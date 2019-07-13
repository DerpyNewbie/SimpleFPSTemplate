// Fill out your copyright notice in the Description page of Project Settings.

#include "MyBlackholeActor.h"
#include "Components/PrimitiveComponent.h"
#include "Components/StaticMeshComponent.h"
#include "Components/SphereComponent.h"

// Sets default values
AFPSGameForceActor::AFPSGameForceActor()
{
	// Set this actor to call Tick() every frame.  You can turn this off to improve performance if you don't need it.
	PrimaryActorTick.bCanEverTick = true;
	// Default values
	ForceStrength = -5000;
	ForceRadius = 5000;
	RadialImpulseType = ERadialImpulseFalloff::RIF_Constant;
	AccelCharge = true;

	// Setup blackhole
	MeshComp = CreateDefaultSubobject<UStaticMeshComponent>(TEXT("MeshComp0"));
	MeshComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	MeshComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	RootComponent = MeshComp;

	// Setup "may the force be with you"
	SphereComp = CreateDefaultSubobject<USphereComponent>(TEXT("SphereComp0"));
	SphereComp->SetCollisionEnabled(ECollisionEnabled::QueryOnly);
	SphereComp->SetCollisionResponseToAllChannels(ECollisionResponse::ECR_Overlap);
	SphereComp->SetupAttachment(MeshComp);

}

// Called when the game starts or when spawned
void AFPSGameForceActor::BeginPlay()
{
	Super::BeginPlay();

}

// Called every frame
void AFPSGameForceActor::Tick(float DeltaTime)
{
	Super::Tick(DeltaTime);
	// Get overlapping components
	MeshComp->GetOverlappingComponents(MeshOverlapComps);
	SphereComp->GetOverlappingComponents(SphereOverlapComps);
	// Destroy AActor that is overlapping MeshComp
	for (UPrimitiveComponent* meshOverlapComp : MeshOverlapComps)
	{
		if (meshOverlapComp->GetOwner() != MeshComp->GetOwner())
			meshOverlapComp->GetOwner()->Destroy();
	}
	// Apply force to blocks
	for (UPrimitiveComponent* sphereOverlapComp : SphereOverlapComps)
	{
		sphereOverlapComp->AddRadialForce(GetActorLocation(), ForceRadius, ForceStrength, RadialImpulseType, AccelCharge);
	}
}
