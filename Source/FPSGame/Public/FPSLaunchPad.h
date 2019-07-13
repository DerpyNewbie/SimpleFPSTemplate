// Fill out your copyright notice in the Description page of Project Settings.

#pragma once

#include "CoreMinimal.h"
#include "GameFramework/Actor.h"
#include "FPSLaunchPad.generated.h"

UCLASS()
class FPSGAME_API AFPSLaunchPad : public AActor
{
	GENERATED_BODY()

public:
	// Sets default values for this actor's properties
	AFPSLaunchPad();

protected:

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
		class UBoxComponent* BoxComp;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
		class UStaticMeshComponent* BasePlateComp;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
		class UStaticMeshComponent* ArrowTextureComp;

	UPROPERTY(EditDefaultsOnly, Category = "LaunchPad")
		class UParticleSystem* ParticleEffect;

	UPROPERTY(EditAnywhere, Category = "LaunchPad", meta = (ToolTip = "Length of velocity vector"))
		float LaunchForce;

	UPROPERTY(EditAnywhere, Category = "LaunchPad", meta = (ToolTip = "Angle of velocity vector"))
		float LaunchPitch;

	/* Advanced Parameters */

	UPROPERTY(AdvancedDisplay, EditAnywhere, Category = "LaunchPad")
		float LaunchYaw;

	UPROPERTY(AdvancedDisplay, EditAnywhere, Category = "LaunchPad")
		float LaunchRoll;

	UPROPERTY(AdvancedDisplay, EditDefaultsOnly, Category = "LaunchPad", meta = (ToolTip = "If true, LaunchPad will calculate velocity each launching."))
		bool bDynamicVelocityCalculation;

	UPROPERTY(AdvancedDisplay, EditDefaultsOnly, Category = "LaunchPad", meta = (ToolTip = "If true, LaunchPad will draw line to Velocity Vector.", EditCondition = bDynamicVelocityCalculation))
		bool bDrawDebugLine;

	UPROPERTY(AdvancedDisplay, EditDefaultsOnly, Category = "LaunchPad", meta = (ToolTip = "If true, LaunchPad will override other actor's velocity."))
		bool bPreventOtherActorVelocity;

	UPROPERTY(AdvancedDisplay, EditDefaultsOnly, Category = "LaunchPad", meta = (ToolTip = "If true, LaunchPad will use ACharacter::LaunchCharacter to launch characters. this setting will fix problems with character not launching issue."))
		bool bUseLaunchCharacter;

	UPROPERTY(AdvancedDisplay, EditDefaultsOnly, Category = "LaunchPad", meta = (ToolTip = "If true, LaunchPad will check if target's physics are simulated or not. if true, this setting will prevent physics warnings."))
		bool bCheckSimlatingPhysics;

	UPROPERTY(AdvancedDisplay, VisibleAnywhere, BlueprintReadOnly, Category = "LaunchPad", meta = (ShortTooltip = "An calculated velocity", ToolTip = "Velocity calculated using Actor Location, LaunchForce and LaunchAngle."))
		FVector VelocityVector;

	/* Legacy Parameters */

	UPROPERTY(AdvancedDisplay, EditAnywhere, Category = "LaunchPad", meta = (ToolTip = "Enable this to use old calculation"))
		bool bUseLegacy;

	UPROPERTY(AdvancedDisplay, EditAnywhere, Category = "LaunchPad", meta = (EditCondition = bUseLegacy, MakeEditWidget = true))
		FVector LegacyVelocityLocation;

public:

	UFUNCTION()
		virtual void BeginPlay() override;

	UFUNCTION()
		virtual void HandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

	UFUNCTION()
		virtual void LegacyHandleBeginOverlap(UPrimitiveComponent* OverlappedComponent, AActor* OtherActor, UPrimitiveComponent* OtherComp, int32 OtherBodyIndex, bool bFromSweep, const FHitResult& SweepResult);

private:

	void playEffect();

	bool primCompCheck(UPrimitiveComponent* primComp);

	void calculateVelocity();

};
