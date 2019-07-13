// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
//#include "Engine/EngineTypes.h"
#include "MyBlackholeActor.generated.h"

UCLASS()
class FPSGAME_API AFPSGameForceActor : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSGameForceActor();

protected:

	UPROPERTY(EditInstanceOnly, Category = "Components")
		class UStaticMeshComponent* MeshComp;

	UPROPERTY(EditInstanceOnly, Category = "Components")
		class USphereComponent* SphereComp;

	UPROPERTY(EditInstanceOnly, Category = "Components")
		float ForceStrength;

	UPROPERTY(EditInstanceOnly, Category = "Components")
		float ForceRadius;

	UPROPERTY(EditInstanceOnly, Category = "Components")
		TEnumAsByte<enum ERadialImpulseFalloff> RadialImpulseType;

	UPROPERTY(EditInstanceOnly, Category = "Components")
		bool AccelCharge;

	class TSet<UPrimitiveComponent*> MeshOverlapComps;
	class TSet<UPrimitiveComponent*> SphereOverlapComps;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;
};