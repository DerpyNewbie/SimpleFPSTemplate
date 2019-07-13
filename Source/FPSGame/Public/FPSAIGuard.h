// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Character.h"
#include "FPSAIGuard.generated.h"

UENUM(BlueprintType)
enum class EAIState : uint8
{
	Idle,
	Suspicious,
	Alerted,
};


UCLASS()
class FPSGAME_API AFPSAIGuard : public ACharacter
{
	GENERATED_BODY()

public:
	// Sets default values for this character's properties
	AFPSAIGuard();

protected:

	UPROPERTY(ReplicatedUsing = OnRep_GuardState)
		EAIState GuardState;

	FRotator OriginalRotation;

	FTimerHandle TimerHandle;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		bool bDrawSeenSphere;

	UPROPERTY(EditDefaultsOnly, Category = "Components")
		bool bDrawHeardSphere;

	UPROPERTY(VisibleAnywhere, Category = "Components")
		class UPawnSensingComponent* PawnSensingComp;

	// Called when the game starts or when spawned
	virtual void BeginPlay() override;

	UFUNCTION()
		void OnPawnSeen(APawn* SeenPawn);

	UFUNCTION()
		void OnNoiseHeard(APawn* NoisyPawn, const FVector& Location, float Volume);

	UFUNCTION()
		void ResetOrientation();

	UFUNCTION(BlueprintImplementableEvent, Category = "AI")
		void OnStateChange(EAIState NewState);

	UFUNCTION()
		void OnRep_GuardState();

public:
	// Called every frame
	virtual void Tick(float DeltaTime) override;

	void SetGuardState(EAIState NewState);

};
